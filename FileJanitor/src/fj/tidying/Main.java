package fj.tidying;

import fj.Find.FoundCallback;
import fj.TreeWalkingFiltre;
import java.nio.file.Path;
import java.nio.file.Paths;
import static fj.Find.find;

public class Main {

	private static StringBuilder fuckupreason = new StringBuilder(4096);
	private static String fuckupfile = null;
	public static void FuckupFile (final String file) {
		assert fuckupfile == null;
		fuckupfile = file;
	}
	public static void AddFuckupReason (final String reason) {
		assert fuckupfile != null;
		fuckupreason.append(" [").append(reason).append(']');
	}
	public static void DoneFuckingUpFile () {
		assert fuckupfile != null;
		System.out.printf("Fucked up file %s because:%s%n", fuckupfile, fuckupreason);
		fuckupfile = null;
		fuckupreason.delete(0, fuckupreason.length());
	}
	
	private static final TreeWalkingFiltre toBeTidiedFiltre = new ToBeTidiedFiltre();
	private static final FoundCallback fileTidyingCallback = new FileTidyingCallback();
	
	public static void main (String[] args) throws Throwable {
		final Path cwd = Paths.get(args[0]);
		System.out.printf("Tidying up in directory: %s%n", cwd.toRealPath());
		find(cwd, toBeTidiedFiltre, fileTidyingCallback);
	}

}
