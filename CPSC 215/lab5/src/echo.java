import java.io.BufferedReader;
import java.io.FileReader;
import java.io.IOException;
import java.io.InputStreamReader;
import java.net.MalformedURLException;
import java.net.URL;

public class echo {
	/**
	 * @param args 
	 */
	public static void main(String[] args) {
		try{
			if(args.length != 1){
				System.out.println("No file specified. Exiting...");
				System.exit(0);
			}
			String inFile = args[0];
			BufferedReader inBuffer;
			if (inFile.substring(0,7).equals("http://")){
				inBuffer = new BufferedReader(new InputStreamReader(new URL(inFile).openConnection().getInputStream()));	
			}
			else{
				inBuffer = new BufferedReader(new FileReader(inFile));
			}
			String line = inBuffer.readLine();
			while(line != null){
				System.out.println(line);
				line = inBuffer.readLine();
			}
		}
		catch (MalformedURLException e) {
			System.out.println("Error openeing URL. Please check you address is correct.");
		}
		catch (IOException e){
			System.out.println("Error reading a file somewhere, either on disk or over the network. Sorry I couldn't be more specific.");
		}
	}
}