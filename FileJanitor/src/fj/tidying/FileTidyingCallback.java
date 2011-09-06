package fj.tidying;

import fj.util.FoundCallback;
import java.io.BufferedReader;
import java.io.IOException;
import java.io.StringReader;
import java.nio.ByteBuffer;
import java.nio.channels.SeekableByteChannel;
import java.nio.charset.Charset;
import java.nio.file.Files;
import java.nio.file.Path;
import java.nio.file.StandardOpenOption;
import java.util.List;
import static fj.tidying.Main.RecordBeginFuckingUpFile;
import static fj.tidying.Main.AddFuckingUpReason;
import static fj.tidying.Main.RecordDoneFuckingUpFile;
import static fj.tidying.Main.FuckUpFile;

class FileTidyingCallback implements FoundCallback {

	@Override
	public void run (final Path file) {
		final String resolt;
		final List<String> reasons;
		final Charset charset;

		try (@SuppressWarnings("NestedAssignment")
			final BufferedReader reader = Files.newBufferedReader(file, charset = DetectCharset(file)))
		{
			final FileTidier fileTidier = new FileTidier(reader);
			resolt = fileTidier.ReadAndProduceTidy().GetResolt();
			reasons = fileTidier.GetReasons();

			assert new FileTidier(new StringReader(resolt)).ReadAndProduceTidy().GetReasons().isEmpty();
		} catch (final IOException io) {
			throw new RuntimeException(io);
		}

		if (!reasons.isEmpty()) {
			RecordBeginFuckingUpFile(file.toString());
			for (final String reason : reasons)
				AddFuckingUpReason(reason);
			RecordDoneFuckingUpFile();

			FuckUpFile(file, charset, resolt);
		}
	}

	private Charset DetectCharset (final Path file) throws IOException {
		final ByteBuffer bbuf = ByteBuffer.allocate(2);
		final int bytesread;
		try (final SeekableByteChannel bch = Files.newByteChannel(file, StandardOpenOption.READ)) {
			bytesread = bch.read(bbuf);
		}
		bbuf.flip();

		final Charset defaultcharset = Charset.forName("UTF-8");
		final Charset candidate;
		switch (bytesread) {
			case 2: {
				final byte b0 = bbuf.get();
				final byte b1 = bbuf.get();
				if (b0 == (byte)0xfe && b1 == (byte)0xff)
					candidate = Charset.forName("UTF-16BE");
				else
				if (b0 == (byte)0xff && b1 == (byte)0xfe)
					candidate = Charset.forName("UTF-16LE");
				else
					candidate = defaultcharset;
				break;
			}
			default:
				candidate = defaultcharset;
		}

		final Charset fallbackcharset = Charset.forName("ISO-8859-1");
		Charset resolt;
		try (final BufferedReader reada = Files.newBufferedReader(file, candidate)) {
			reada.skip(Integer.MAX_VALUE);
			resolt = candidate;
		}
		catch (final java.nio.charset.MalformedInputException e) {
			resolt = fallbackcharset;
		}

		return resolt;
	}

}
