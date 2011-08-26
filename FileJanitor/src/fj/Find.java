package fj;

import java.io.IOException;
import java.nio.file.DirectoryStream;
import java.nio.file.Files;
import java.nio.file.Path;

public class Find {

	public static interface FoundCallback {
		void run (final Path file);
	}

	public static void find (final Path root, final TreeWalkingFiltre filtre, FoundCallback fc) throws IOException {
		if (!Files.isDirectory(root))
			throw new RuntimeException("Root not a dir: " + root);
		try (DirectoryStream<Path> ds = Files.newDirectoryStream(root, filtre)) {
			for (final Path child : ds)
				if (!Files.isDirectory(child) || filtre.acceptDir(child))
					fc.run(child);
				else
					find(child, filtre, fc);
		}
	}


	private Find () {
	}

}
