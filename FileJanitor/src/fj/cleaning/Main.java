package fj.cleaning;

import java.util.ArrayList;
import java.io.BufferedReader;
import java.io.IOException;
import java.nio.file.Files;
import fj.Find.FoundCallback;
import java.nio.charset.Charset;
import java.nio.file.Path;
import java.util.Formatter;
import static fj.Find.find;
import static fj.MainUtils.GetArg;
import static java.nio.file.Paths.get;
import static java.nio.file.Files.newBufferedReader;
import static fj.MainUtils.GetDefaultCharset;
import static fj.MainUtils.println;

public class Main {
	private final static StringBuilder	fout= new StringBuilder(1024);
	private final static Formatter		f	= new Formatter(fout);
	public static void delete (final Path file) throws IOException {
		fout.delete(0, fout.length());
		if (dry)
			f.format("Would delete %s", file);
		else {
			f.format("Deleting %s", file);
			Files.delete(file);
		}

		println(f.toString());
	}

	private static final Charset		defaultCharset			= GetDefaultCharset();
	private static final FoundCallback	treeDeletingCallback	= new DeletingTreeCallback(new TreeDeleter());
	private static boolean				dry						= false;

	public static void SetDryRun () {
		dry = true;
	}

	public static void main (ArrayList<String> args) throws Throwable {
		final Path cwd = get(args.get(0));
		final BufferedReader configReader = newBufferedReader(get(GetArg(args, "clean:", "FileJanitor_cleaning.config")), defaultCharset);
		System.out.printf("Cleaning up in directory: %s%n", cwd.toRealPath());
		find(cwd, new ToBeDeletedFiltre(configReader), treeDeletingCallback);
	}

}
