package fj.tidying;

import fj.TreeWalkingFiltre;
import java.nio.file.FileSystems;

public final class ToBeTidiedFiltre extends TreeWalkingFiltre {

	public ToBeTidiedFiltre () {
		super(
				FileSystems.getDefault().getPathMatcher("glob:*.{h,cpp}"),
				FileSystems.getDefault().getPathMatcher("glob:")
			);
	}
	
}
