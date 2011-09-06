package fj.util;

import java.io.IOException;
import java.nio.file.DirectoryStream.Filter;
import java.nio.file.Files;
import java.nio.file.Path;
import java.nio.file.PathMatcher;

public abstract class TreeWalkingFiltre implements Filter<Path> {

	private final PathMatcher	fileMatcher;
	private final PathMatcher	dirMatcher;
	public TreeWalkingFiltre (final PathMatcher fileMatcher, final PathMatcher dirMatcher) {
		this.fileMatcher = fileMatcher;
		this.dirMatcher = dirMatcher;
	}

	@Override
	public boolean accept (final Path entry) throws IOException {
		final boolean isdir = Files.isDirectory(entry);
		final boolean isnothidden = !Files.isHidden(entry);
		final boolean isnotdothidden = !entry.getFileName().toString().startsWith(".");
		final boolean matchesglob = fileMatcher.matches(entry.getFileName());
		final boolean result = (isdir && isnothidden && isnotdothidden) || (isnothidden && isnotdothidden && matchesglob);
		return result;
	}

	public boolean acceptDir (final Path dir) {
		if (!Files.isDirectory(dir)) {
			throw new RuntimeException("Dir [" + dir + "] is not a directory");
		}
		final Path last = dir.getFileName();
		final boolean matchesglob = dirMatcher.matches(last);
		final boolean result = matchesglob;
		return result;
	}

}
