package fj.util;

import java.util.ArrayList;
import java.util.Arrays;

public class Collections {

	public static <T> ArrayList<T> list(T[] arr) {
		return new ArrayList<>(Arrays.asList(arr));
	}

	private Collections() {
	}

}
