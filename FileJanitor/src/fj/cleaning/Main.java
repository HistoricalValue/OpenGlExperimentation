package fj.cleaning;

import java.nio.file.Paths;
import java.util.ArrayList;
import java.io.BufferedReader;
import java.io.IOException;
import java.nio.file.Files;
import fj.util.FoundCallback;
import java.nio.charset.Charset;
import java.nio.file.Path;
import java.util.Formatter;
import static fj.util.Find.find;
import static fj.Main.printf;
import static fj.MainUtils.GetDefaultCharset;
import static fj.Main.println;

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

	private static void Clean (final Path path, final ToBeDeletedFiltre filtre) throws IOException {
		printf("Cleaning up in directory: %s%n", path);
		find(path, filtre, treeDeletingCallback);
	}

	public static void main (final BufferedReader configReader, ArrayList<String> args) throws Throwable {
		final ToBeDeletedFiltre filtre = new ToBeDeletedFiltre(configReader);

		for (final String pathstr: args)
			Clean(Paths.get(pathstr).toRealPath(), filtre);
	}

	private Main() {}
}
