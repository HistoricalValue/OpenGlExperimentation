package fj.tidying;

import fj.ConfigurableFiltre;
import java.io.BufferedReader;
import java.io.IOException;

public class ToBeTidiedFiltre extends ConfigurableFiltre {

	public ToBeTidiedFiltre (final BufferedReader reader) throws IOException {
		super(reader);
	}

}
