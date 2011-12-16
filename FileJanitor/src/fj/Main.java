package fj;

import java.io.BufferedReader;
import java.util.ArrayList;
import java.util.Arrays;
import java.nio.file.Paths;
import static fj.MainUtils.GetArg;
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


		boolean withCleaning = false;
		boolean withTidying = false;

		if (modargs.remove("--clean"))
			withCleaning = true;
		if (modargs.remove("--tidy"))
			withTidying = true;
		if (modargs.remove("--all")) {
			withCleaning = true;
			withTidying = true;
		}

		final BufferedReader cleanConfigReader = newBufferedReader(Paths.get(GetArg(modargs, "clean:", "FileJanitor_cleaning.config")), GetDefaultCharset());
		final BufferedReader tidyConfigReader = newBufferedReader(Paths.get(GetArg(modargs, "tidy:", "FileJanitor_tidying.config")), GetDefaultCharset());

		if (withCleaning)
			fj.cleaning.Main.main(cleanConfigReader, modargs);
		if (withTidying)
			fj.tidying.Main.main(tidyConfigReader, modargs);

		if (!withCleaning && !withTidying)
			println("No command. Try --clean, --tidy or --all");
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
