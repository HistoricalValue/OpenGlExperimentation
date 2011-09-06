package fj;

import java.nio.charset.Charset;
import java.nio.file.Path;
import static java.util.Arrays.asList;

/**
 *
 * @author muhtaris
 */
public class MainUtils {
	private MainUtils () {}

	public static <T> T Default (final T obj, final T default_) {
		return obj != null? obj : default_;
	}

	public static String GetArg (final String[] args, final String pref, final String default_) {
		return GetArg(asList(args), pref, default_);
	}
	
	public static String GetArg (final Iterable<? extends String> args, final String pref, final String default_) {
		String result = null;

		for (final String arg : args)
			if (arg.startsWith(pref))
				result = arg.substring(pref.length());

		return Default(result, default_);
	}
	
	private static final Charset defaultCharset = Charset.forName("UTF-8");
	public static Charset GetDefaultCharset () {
		return defaultCharset;
	}
	
	public static void println (final String str) {
		System.out.println(str);
	}
}
