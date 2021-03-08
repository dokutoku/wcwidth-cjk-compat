module cmd;


private static import core.memory;
private static import core.stdc.locale;
private static import core.stdc.stdio;
private static import core.stdc.stdlib;
private static import core.stdc.string;
private static import core.stdc.wchar_;
private static import std.string;
private static import wcwidth_cjk_compat.wcwidth;

nothrow @nogc @live
private void dump_bigendian(core.stdc.stdio.FILE* fp, scope const void* p_in, size_t size)

	do
	{
		size_t i = 0;

		for (ubyte* p = cast(ubyte*)(p_in); i < size; i++) {
			core.stdc.stdio.fprintf(fp, " %02X", *(p + i));
		}
	}

nothrow @nogc @live
private void dump_littleendian(core.stdc.stdio.FILE* fp, scope const void* p_in, size_t size)

	do
	{
		int i = cast(int)(size) - 1;

		for (ubyte* p = cast(ubyte*)(p_in); i >= 0; i--) {
			core.stdc.stdio.fprintf(fp, " %02X", *(p + i));
		}
	}

int main(string[] argv)

	do
	{
		if (argv.length < 2) {
			core.stdc.stdio.printf("Usage: %s STRING [...]\n", std.string.toStringz(argv[0]));

			return 1;
		}

		core.stdc.locale.setlocale(core.stdc.locale.LC_ALL, "");
		core.stdc.stdlib.mbtowc(null, null, 0);

		char* mb_buf = cast(char*)(core.memory.pureMalloc(core.stdc.stdlib.MB_CUR_MAX + 1));

		if (mb_buf == null) {
			core.stdc.stdio.fputs("allocation error.\n", core.stdc.stdio.stderr);

			return 1;
		}

		scope (exit) {
			assert(mb_buf != null);
			core.memory.pureFree(mb_buf);
			mb_buf = null;
		}

		uint wc = void;

		for (size_t i = 1; i < argv.length; i++) {
			immutable (char)* mb_p = std.string.toStringz(argv[i]);
			size_t mb_len = core.stdc.string.strlen(mb_p);

			while (mb_len > 0) {
				core.stdc.wchar_.wchar_t wc_temp = void;
				int mb_consumed = core.stdc.stdlib.mbtowc(&wc_temp, mb_p, mb_len);

				if (mb_consumed == -1) {
					core.stdc.stdio.fprintf(core.stdc.stdio.stderr, "%s: ERROR: Invalid multibyte sequence: argv=%zd, index=%d, bytes:", std.string.toStringz(argv[0]), i, cast(int)(mb_p - std.string.toStringz(argv[i])));
					.dump_bigendian(core.stdc.stdio.stderr, mb_p, mb_len);
					core.stdc.stdio.fputs("\n", core.stdc.stdio.stderr);

					return 1;
				}

				wc = wc_temp;
				int wc_width = wcwidth_cjk_compat.wcwidth.wcwidth_cjk(wc);

				core.stdc.string.strncpy(mb_buf, mb_p, mb_consumed);
				mb_buf[mb_consumed] = '\0';

				core.stdc.stdio.printf("%d", wc_width);

				version (BigEndian) {
					.dump_bigendian(core.stdc.stdio.stdout, &wc, uint.sizeof);
				} else {
					.dump_littleendian(core.stdc.stdio.stdout, &wc, uint.sizeof);
				}

				core.stdc.stdio.printf("\t%s\n", mb_buf);

				mb_p += mb_consumed;
				mb_len -= mb_consumed;
			}
		}

		return 0;
	}
