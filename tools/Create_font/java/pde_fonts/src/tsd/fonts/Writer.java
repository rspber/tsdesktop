/*
  pde_fonts

  Copyright (c) 2022, rspber (https://github.com/rspber)

*/

package tsd.fonts;

public class Writer {

	StringBuilder sb = new StringBuilder();
	
	public void write(String format, Object... args)
	{
		sb.append(String.format(format, args));
	}
}
