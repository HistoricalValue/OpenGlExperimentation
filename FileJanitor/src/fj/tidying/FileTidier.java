package fj.tidying;

import java.io.IOException;
import java.io.StringReader;
import java.nio.CharBuffer;
import java.util.LinkedList;
import java.util.List;
import java.util.regex.Matcher;
import java.util.regex.Pattern;

public class FileTidier {
	
	private final static int		CAP32KiB				= 1 << 16;
	private final static Pattern	TrailingSpacesRegexp	= Pattern.compile("[ \\t]+$");
	private final StringBuilder		bob						= new StringBuilder(CAP32KiB);
	private final CharBuffer		buf						= CharBuffer.allocate(CAP32KiB);
	private Readable				r;
	private boolean			hasNotAddedDosFormatReason		= true;
	private boolean			hasNotAddedTrailingSpacesReason	= true;
	private final List<String>		reasons					= new LinkedList<>();
	
	public List<String> GetReasons () {
		return reasons;
	}
	
	public FileTidier (final Readable r) {
		this.r = r;
		buf.clear();
		buf.position(0);
		buf.limit(0);
	}

	public FileTidier ReadAndProduceTidy () throws IOException {
		UnixFormat();
		TrailingSpaces();
		return this;
	}
	
	public String GetResolt () {
		return bob.toString();
	}
	
	private void TrailingSpaces () throws IOException {
		resetbob();
		while (!eof()) {
			final String line = readline();
			final Matcher matcher = TrailingSpacesRegexp.matcher(line);
			if (matcher.find())
				appendtrailingspacesreason();
			final String linenotrailin = matcher.replaceAll("\n");
			bob.append(linenotrailin);
		}
	}
	
	private void UnixFormat () throws IOException {
		resetbob();
		while (!eof()) {
			final char c = nextchar();
			if (c != '\r')
				bob.append(c);
			else
				appenddosformatreason();
		}
		r = new StringReader(bob.toString());
	}
	
	private boolean eof () throws IOException {
		boolean result;
		if (buf.remaining() > 0)
			result = false;
		else {
			buf.clear();
			int charsread;
			for (charsread = r.read(buf); charsread == 0; charsread = r.read(buf))
				buf.clear();
			
			buf.position(0);
			result = charsread == -1;
			
			if (result)
				buf.limit(0);
			else
				buf.limit(charsread);
		}
		return result;
	}
	
	private char nextchar () throws IOException {
		if (eof())
			throw new RuntimeException("EOF");
		
		return buf.get();
	}
	
	private String readline () throws IOException {
		if (eof())
			throw new RuntimeException("EOF");
		
		final StringBuilder b = new StringBuilder(1024);
		
		char c;
		for (c = nextchar(); c != '\n' && !eof(); c = nextchar())
			b.append(c);
		
		if (c == '\n')
			b.append(c);
		
		return b.toString();
	}
	
	private void resetbob () {
		bob.delete(0, bob.length());
	}

	private void appenddosformatreason () {
		if (hasNotAddedDosFormatReason) {
			hasNotAddedDosFormatReason = false;
			reasons.add("dos format");
		}
	}
	
	private void appendtrailingspacesreason () {
		if (hasNotAddedTrailingSpacesReason) {
			hasNotAddedTrailingSpacesReason = false;
			reasons.add("trailing spaces");
		}
	}
}
