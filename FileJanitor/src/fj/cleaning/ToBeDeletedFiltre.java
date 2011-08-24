package fj.cleaning;

import fj.TreeWalkingFiltre;
import java.nio.file.FileSystems;

final public class ToBeDeletedFiltre extends TreeWalkingFiltre {
	
	public ToBeDeletedFiltre () {
		super(
				FileSystems.getDefault().getPathMatcher("glob:*.{sdf,asm,obj,ncb,pdb,aps}"),
				FileSystems.getDefault().getPathMatcher("glob:{Debug,Release}")
			);
	}
	
}
