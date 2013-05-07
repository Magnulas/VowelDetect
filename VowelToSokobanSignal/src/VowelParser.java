import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;
import java.util.HashMap;

public class VowelParser {


	public static void main(String args[]) throws IOException, InterruptedException{

		BufferedReader in = new BufferedReader(new InputStreamReader(System.in));
		boolean run = true;
		
		HashMap<String, String> vowelSignal = new HashMap<String,String>();

		vowelSignal.put("A", "U");
		vowelSignal.put("O", "D");
		vowelSignal.put("AO", "L");
		vowelSignal.put("I", "R");
		
		while(run){
			if(in.ready()){
				String word = in.readLine();
				if(vowelSignal.containsKey(word)){
					System.out.println(vowelSignal.get(word));
				}
			}
		}
	}
}
