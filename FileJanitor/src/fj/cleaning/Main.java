package fj.cleaning;

import java.io.IOException;
import java.nio.file.Files;
import fj.Find.FoundCallback;
import fj.TreeWalkingFiltre;
import java.nio.file.Path;
import java.nio.file.Paths;
import static fj.Find.find;

public class Main {

	public static void delete (final Path file) throws IOException {
		System.out.printf("Deleting %s%n", file);
		Files.delete(file);
	}

	private static final TreeWalkingFiltre toBeDeletedFiltre = new ToBeDeletedFiltre();
	private static final FoundCallback treeDeletingCallback = new DeletingTreeCallback(new TreeDeleter());

	public static void main (String[] args) throws Throwable {
		final Path cwd = Paths.get(args[0]);
		System.out.printf("Cleaning up in directory: %s%n", cwd.toRealPath());
		find(cwd, toBeDeletedFiltre, treeDeletingCallback);
	}

}
