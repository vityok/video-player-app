
.PHONY : clean

build : omx-vala

omx-vala : omx-vala.vala
	valac --pkg gtk+-3.0 --pkg glib-2.0 --pkg posix omx-vala.vala

clean:
	rm omx-vala *~
