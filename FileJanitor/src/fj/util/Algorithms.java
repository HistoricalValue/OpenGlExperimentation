package fj.util;

public class Algorithms {

	public static <T> int IndexOf (Iterable<? extends T> iterable, Predicate<? super T> pred) {
		boolean found = false;
		int i = 0;
		for (final T o : iterable) {
			found = pred.accept(o);
			if (found) {
				break;
			} else {
				++i;
			}
		}
		return found ? i : -1;
	}

	private Algorithms() {}
}
