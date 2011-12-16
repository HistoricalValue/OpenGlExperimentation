package fj.tidying;

import java.nio.file.Paths;
import java.io.BufferedWriter;
import java.io.IOException;
import java.nio.file.Files;
import java.nio.file.StandardOpenOption;
import java.util.ArrayList;
import java.nio.file.Path;
import fj.util.FoundCallback;
import java.io.BufferedReader;
import java.nio.charset.Charset;
import static fj.util.Find.find;
import static fj.Main.print;
import static fj.Main.printf;
import static fj.MainUtils.GetDefaultCharset;

public class Main {

	private static StringBuilder fuckupreason = new StringBuilder(4096);
	private static String fuckupfile = null;
	static void RecordBeginFuckingUpFile (final String file) {
		assert fuckupfile == null;
		fuckupfile = file;
	}
	static void AddFuckingUpReason (final String reason) {
		assert fuckupfile != null;
		fuckupreason.append(" [").append(reason).append(']');
	}
	static void RecordDoneFuckingUpFile () {
		assert fuckupfile != null;
		if (dry)
			print("Would fuck ");
		else
			print("Fucked ");
		printf("up file %s because:%s%n", fuckupfile, fuckupreason);
		fuckupfile = null;
		fuckupreason.delete(0, fuckupreason.length());
	}
	static void FuckUpFile (final Path file, final Charset charset, final String resolt) {
		if (!dry)
			try (final BufferedWriter w = Files.newBufferedWriter(file, charset, StandardOpenOption.TRUNCATE_EXISTING)) {
				w.append(resolt);
			}
			catch (final IOException io) {
				throw new RuntimeException(io);
			}
	}


	private static final Charset		defaultCharset		= GetDefaultCharset();
	private static final FoundCallback	fileTidyingCallback	= new FileTidyingCallback();
	private static boolean				dry					= false;

	public static void SetDryRun () {
		dry = true;
	}

	private static void Tidy (final Path path, final ToBeTidiedFiltre filtre) throws IOException {
		printf("Tidying up in directory: %s%n", path);
		find(path, filtre, fileTidyingCallback);
	}

	public static void main (final BufferedReader configReader, ArrayList<String> args) throws Throwable {
		final ToBeTidiedFiltre filtre = new ToBeTidiedFiltre(configReader);

		for (final String pathstr: args)
			Tidy(Paths.get(pathstr).toRealPath(), filtre);
	}

	private Main() {}
}
