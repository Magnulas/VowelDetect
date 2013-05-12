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

		int timeStep = 100;
		int minOutputSpeed = 100;
		int maxOutputSpeed = 500; //milliseconds
		int outputSpeed = maxOutputSpeed;

		String lastSignal = null;
		long previousTime = 0;

		while(run){
			if(in.ready()){
				String vowel = in.readLine();
				long currentTime = System.currentTimeMillis();
				if(currentTime-previousTime>=outputSpeed){ //Check timelimit passed
					if(vowelSignal.containsKey(vowel)){ //Check vowel valid
						String signal = vowelSignal.get(vowel); //Get signal
						System.out.println(signal); //send signal

						if(signal == lastSignal){ //check if vowel same as last, increase speed of output
							outputSpeed = Math.min(outputSpeed-timeStep, minOutputSpeed);
						} else{ //Signal not the same, reset output speed
							outputSpeed = maxOutputSpeed;
						}
						lastSignal = signal; //update previous signal
						previousTime = currentTime; //Update previous time
					} else{ //Vowel not valid, reset timer
						outputSpeed = maxOutputSpeed;
					}
				}
			}
		}
	}
}
