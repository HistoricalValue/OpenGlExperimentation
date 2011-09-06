package fj;

import fj.util.TreeWalkingFiltre;
import java.io.BufferedReader;
import java.io.IOException;
import java.nio.file.FileSystems;

public class ConfigurableFiltre extends TreeWalkingFiltre {

	public ConfigurableFiltre (final BufferedReader reader) throws IOException {
		super(
				FileSystems.getDefault().getPathMatcher(reader.readLine()),
				FileSystems.getDefault().getPathMatcher(reader.readLine())
			);
	}

}
