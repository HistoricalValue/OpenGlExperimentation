package fj;

import java.io.BufferedReader;
import java.util.ArrayList;
import java.util.Arrays;
import static fj.MainUtils.GetArg;
import static java.nio.file.Paths.get;
import static java.nio.file.Files.newBufferedReader;
import static fj.MainUtils.GetDefaultCharset;

public class Main {

	public static void main (final String[] args) throws Throwable {
		final ArrayList<String> modargs = new ArrayList<>(args.length);
		modargs.addAll(Arrays.asList(args));

		if (modargs.remove("--dry")) {
			fj.cleaning.Main.SetDryRun();
			fj.tidying.Main.SetDryRun();
		}
		final BufferedReader cleanConfigReader = newBufferedReader(get(GetArg(modargs, "clean:", "FileJanitor_cleaning.config")), GetDefaultCharset());
		final BufferedReader tidyConfigReader = newBufferedReader(get(GetArg(modargs, "tidy:", "FileJanitor_tidying.config")), GetDefaultCharset());

		fj.cleaning.Main.main(cleanConfigReader, modargs);
		fj.tidying.Main.main(tidyConfigReader, modargs);
	}

	@SuppressWarnings("UseOfSystemOutOrSystemErr")
	public static void print (final String str) {
		System.out.print(str);
	}
	@SuppressWarnings("UseOfSystemOutOrSystemErr")
	public static void printf (final String format, final Object... args) {
		System.out.printf(format, args);
	}
	@SuppressWarnings("UseOfSystemOutOrSystemErr")
	public static void println (final String line) {
		System.out.println(line);
	}

	private Main() {}
}
