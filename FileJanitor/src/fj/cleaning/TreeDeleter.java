package fj.cleaning;

import java.io.IOException;
import java.nio.file.FileVisitResult;
import java.nio.file.FileVisitor;
import java.nio.file.Path;
import java.nio.file.attribute.BasicFileAttributes;
import java.util.LinkedList;
import java.util.List;

final public class TreeDeleter implements FileVisitor<Path> {

	public static final class CouldNotBeDeletedEntry {

		public final Path path;
		public final IOException reason;

		public CouldNotBeDeletedEntry(final Path path, final IOException reason) {
			this.path = path;
			this.reason = reason;
		}
	}
	private final List<CouldNotBeDeletedEntry> couldNotBeDeleted = new LinkedList<>();

	List<CouldNotBeDeletedEntry> GetCouldNotBeDeleted() {
		return couldNotBeDeleted;
	}

	@Override
	public FileVisitResult preVisitDirectory(final Path dir, final BasicFileAttributes attrs) throws IOException {
		return FileVisitResult.CONTINUE;
	}

	@Override
	public FileVisitResult visitFile(final Path file, final BasicFileAttributes attrs) throws IOException {
		Main.delete(file);
		return FileVisitResult.CONTINUE;
	}

	@Override
	public FileVisitResult visitFileFailed(final Path file, final IOException exc) throws IOException {
		couldNotBeDeleted.add(new CouldNotBeDeletedEntry(file, exc));
		return FileVisitResult.CONTINUE;
	}

	@Override
	public FileVisitResult postVisitDirectory(final Path dir, final IOException exc) throws IOException {
		if (exc != null) {
			throw exc;
		}
		Main.delete(dir);
		return FileVisitResult.CONTINUE;
	}
	
}
