package fj.tidying;

import fj.Find.FoundCallback;
import java.io.BufferedReader;
import java.io.BufferedWriter;
import java.io.IOException;
import java.io.StringReader;
import java.nio.charset.Charset;
import java.nio.file.Files;
import java.nio.file.Path;
import java.nio.file.StandardOpenOption;
import java.util.List;

class FileTidyingCallback implements FoundCallback {

	@Override
	public void run (final Path file) {
		final String resolt;
		final List<String> reasons;
		final Charset charset = Charset.forName("ISO-8859-1");
		
		try (final BufferedReader reader = Files.newBufferedReader(file, charset)) {
			final FileTidier fileTidier = new FileTidier(reader);
			resolt = fileTidier.ReadAndProduceTidy().GetResolt();
			reasons = fileTidier.GetReasons();
			
			assert new FileTidier(new StringReader(resolt)).ReadAndProduceTidy().GetReasons().isEmpty();
		} catch (final IOException io) {
			throw new RuntimeException(io);
		}
		
		if (!reasons.isEmpty()) {
			Main.FuckupFile(file.toString());
			for (final String reason : reasons)
				Main.AddFuckupReason(reason);
			Main.DoneFuckingUpFile();
			
			try (final BufferedWriter w = Files.newBufferedWriter(file, charset, StandardOpenOption.TRUNCATE_EXISTING)) {
				w.append(resolt);
			} catch (final IOException io) {
				throw new RuntimeException(io);
			}
		}
	}
	
}
