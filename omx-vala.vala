
/** A simple GUI for the command-line omxplayer video player
 * application.
 */
public class OmxPlayerApp : Gtk.Application {

    public OmxPlayerApp () {
        Object (
            application_id: "org.bitbucket.vityok.omx-player",
            flags: ApplicationFlags.HANDLES_OPEN
        );
    }


	private static bool process_line (IOChannel channel, IOCondition condition, string stream_name) {
		if (condition == IOCondition.HUP) {
			stdout.printf ("%s: The fd has been closed.\n", stream_name);
			return false;
		}

		try {
			string line;
			channel.read_line (out line, null, null);
			stdout.printf ("%s: %s", stream_name, line);
		} catch (IOChannelError e) {
			stdout.printf ("%s: IOChannelError: %s\n", stream_name, e.message);
			return false;
		} catch (ConvertError e) {
			stdout.printf ("%s: ConvertError: %s\n", stream_name, e.message);
			return false;
		}

		return true;
	}

	/** Encapsulates information and operations on the omxplayer child
	 * process.
	 */
	public class OmxPlayer : Object {
		Pid player_pid;
		private int standard_input;
		private int standard_output;
		private int standard_error;

		private IOChannel ioc_in;

		public OmxPlayer (int ppid, int stdin, int stdout, int stderr) {
			this.player_pid = ppid;
			this.standard_input = stdin;
			this.standard_output = stdout;
			this.standard_error = stderr;

			// stdin:
			ioc_in = new IOChannel.unix_new (standard_input);

			// stdout:
			IOChannel output = new IOChannel.unix_new (standard_output);
			output.add_watch (IOCondition.IN | IOCondition.HUP, (channel, condition) => {
					return process_line (channel, condition, "stdout");
				});

			// stderr:
			IOChannel error = new IOChannel.unix_new (standard_error);
			error.add_watch (IOCondition.IN | IOCondition.HUP, (channel, condition) => {
					return process_line (channel, condition, "stderr");
				});

			ChildWatch.add (player_pid, (pid, status) => {
					// Triggered when the child indicated by child_pid exits
					Process.close_pid (pid);
					// loop.quit ();
				});
		}

		public bool send_command(string cmd) {
			size_t len;
			try {
				ioc_in.write_chars((char[])cmd, out len);
				ioc_in.flush();
			}
			catch(IOChannelError e) {
				print("Error writing: %s\n", e.message);
			}
			catch(ConvertError e) {
				print("Error writing: %s\n", e.message);
			}
			return true;
		}

		/** Tries to kill the child process - the one that is the
		 * omxplayer. */
		public void quit() {
			Posix.kill(player_pid, Posix.SIGKILL);
		}

	} // end OmxPlayer class

	public OmxPlayer player = null;


	/** Spawns the OMX player in a child process and set-ups I/O
	 * channels to communicate with it.
	 */
	private void runPlayer (File file) {
		try {
			string[] spawn_args = { "omxplayer", "-b", file.get_path() };
			string[] spawn_env = Environ.get ();
			Pid child_pid;

			int standard_input;
			int standard_output;
			int standard_error;

			Process.spawn_async_with_pipes ("/",
											spawn_args,
											spawn_env,
											SpawnFlags.SEARCH_PATH | SpawnFlags.DO_NOT_REAP_CHILD,
											null,
											out child_pid,
											out standard_input,
											out standard_output,
											out standard_error);

			player = new OmxPlayer(child_pid, standard_input, standard_output, standard_error);

			// loop.run ();
		} catch (SpawnError e) {
			stdout.printf ("Error: %s\n", e.message);
		}
	}


	private void send_command (string cmd) {
		if (player != null) {
			player.send_command(cmd);
		}
	}


    /* Dispatch key presses. */
    private bool on_key_pressed (Gtk.Widget source, Gdk.EventKey key) {
        // If the key pressed was q, quit
        if (key.str == "q") {
			send_command("q");
			player.quit();
            Gtk.main_quit ();
			Process.exit(0);
		} else if (key.keyval == 0xff53) {
			// right: "Seek forward 30"
			send_command("\x1b\x5b\x43");
		} else if (key.keyval == 0xff51) {
			// left: "Seek back 30"
			send_command("\x1b\x5b\x44");
		} else if (key.keyval == 0x20) {
			// space
			send_command("p");
        } else {
			stdout.printf ("keyval: 0x%x\n", key.keyval);
		}

        return false;
    }

	// LAUNCH THE APP

    /** Launched without any files to play. @todo: add "Open file..."
	 * dialog window to select (multiple) files to the playlist.
	 */
    protected override void activate () {
        var main_window = new Gtk.ApplicationWindow (this);
        main_window.default_height = 300;
        main_window.default_width = 300;
        main_window.title = "Omx Video Player";
        main_window.key_press_event.connect (on_key_pressed);
        main_window.show_all ();
    }


	/* This is the normal route: launched with files to play. */
	protected override void open (File[] files, string hint) {
		Gtk.ApplicationWindow main_window = new Gtk.ApplicationWindow (this);
        main_window.default_height = 300;
        main_window.default_width = 300;
        main_window.title = "Omx Video Player";
        main_window.key_press_event.connect (on_key_pressed);
        main_window.show_all ();

		// play each file after file
		foreach (File f in files) {
			runPlayer(f);
		}
	}


    public static int main (string[] args) {
        var app = new OmxPlayerApp ();
        return app.run (args);
    }
}