package fj.cleaning;

import fj.ConfigurableFiltre;
import java.io.BufferedReader;
import java.io.IOException;

public class ToBeDeletedFiltre extends ConfigurableFiltre {

	public ToBeDeletedFiltre (final BufferedReader reader) throws IOException {
		super(reader);
	}

}
