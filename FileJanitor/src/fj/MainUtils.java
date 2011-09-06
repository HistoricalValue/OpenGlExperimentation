package fj;

import fj.util.Algorithms;
import fj.util.Predicate;
import java.nio.charset.Charset;
import java.util.ArrayList;

/**
 *
 * @author muhtaris
 */
public class MainUtils {
	private MainUtils () {}

	public static <T> T Default (final T obj, final T default_) {
		return obj != null? obj : default_;
	}

	public static String GetArg (final ArrayList<? extends String> args, final String pref, final String default_) {
		String result = null;

		final Predicate<String> pred = new Predicate<String>() {
			@Override
			public boolean accept (final String str) {
				return str.startsWith(pref);
			}
		};

		final int argindex = Algorithms.IndexOf(args, pred);
		if (argindex != -1)
			result = args.remove(argindex).substring(pref.length());

		return Default(result, default_);
	}

	private static final Charset defaultCharset = Charset.forName("UTF-8");
	public static Charset GetDefaultCharset () {
		return defaultCharset;
	}
}
