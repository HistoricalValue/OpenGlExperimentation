package fj;

import java.util.ArrayList;
import java.util.Arrays;

public class Main {

	public static void main (final String[] args) throws Throwable {
		final ArrayList<String> modargs = new ArrayList<>(args.length);
		modargs.addAll(Arrays.asList(args));

		if (modargs.remove("--dry")) {
			fj.cleaning.Main.SetDryRun();
			fj.tidying.Main.SetDryRun();
		}
		fj.cleaning.Main.main(modargs);
		fj.tidying.Main.main(modargs);
	}
}
