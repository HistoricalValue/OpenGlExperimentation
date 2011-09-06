package fj.cleaning;

import java.io.IOException;
import java.nio.file.Path;

public class CouldNotBeDeletedEntry {
	public final Path path;
	public final IOException reason;

	public CouldNotBeDeletedEntry(final Path path, final IOException reason) {
		this.path = path;
		this.reason = reason;
	}

}
