import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;

/*
 * Joshua Hull (jhull@clemson.edu)
 * CPSC 2310-001 Program 2
 * 
 * This program demonstrates the steps in an eight bit multiplication. The
 * program prompts the user for two numbers between zero and 255. 
 */

public class multiply {

	public static void main(String[] args) {
		int multiplicand = 0;
		int multiplier = 0;
		int c = 0;
		int acc = 0; 
		int mq;
		int mdr;
		String userInput;
		
		BufferedReader input = new BufferedReader(new InputStreamReader(System.in));

		System.out.print("enter multiplicand: ");
		
		try {
			userInput = input.readLine();
			multiplicand = Integer.parseInt(userInput);
			while(multiplicand < 0 || multiplicand > 255){
				System.out.print("Multiplicand must be between 0 and 255\nenter multiplicand: ");
				userInput = input.readLine();
				multiplicand = Integer.parseInt(userInput);
			}
			
			System.out.print("enter multiplier: ");
			userInput = input.readLine();
			multiplier = Integer.parseInt(userInput);
			while(multiplier < 0 || multiplier > 255){
				System.out.print("Multiplicand must be between 0 and 255\nenter multiplicand: ");
				userInput = input.readLine();
				multiplier = Integer.parseInt(userInput);
			}
		} catch (IOException e) {
			System.out.println("Error reading user input.");
			e.printStackTrace();
			System.out.println("Exiting...");
			System.exit(1);
		}
		
		String tmp = String.format("%8s", Integer.toBinaryString(multiplier)).replace(' ', '0');
		System.out.println("\nc and acc set to 0");
		System.out.format("mq set to multiplier    = %3d decimal and %8s binary\n",multiplier, tmp);
		
		tmp = String.format("%8s", Integer.toBinaryString(multiplicand)).replace(' ', '0');
		System.out.format("mdr set to multiplicand = %3d decimal and %8s binary\n",multiplicand, tmp);
		
		System.out.println("---------------------------------------------------");

		mq = multiplier;
		mdr = multiplicand;
		for(int i = 1; i < 9; i++){
			System.out.format("step %d:   ",i);
			System.out.format("%s ",Integer.toBinaryString(c));
			
			tmp = String.format("%8s", Integer.toBinaryString(acc)).replace(' ', '0');
			System.out.format("%s ", tmp);
			
			tmp = String.format("%8s", Integer.toBinaryString(mq)).replace(' ', '0');
			System.out.format("%s\n", tmp);
			//if(((mq) & (0B1)) == 0B1){
			if((mq & 0x1) == 0x1){
				acc = acc + mdr;
				/*c = (acc & 0B100000000) >> 8;
				acc = acc & 0B11111111;*/
				c = (acc & 0x100) >>> 8;
				acc = (acc & 0xff);
				System.out.print("        +   ");
				
				tmp = String.format("%8s", Integer.toBinaryString(mdr)).replace(' ', '0');
				System.out.format("%s        ^ add based on lsb=1", tmp);
			} else {
				System.out.print("        +   ");
				
				tmp = String.format("%8s", Integer.toBinaryString(0)).replace(' ', '0');
				System.out.format("%s        ^ no add based on lsb=0", tmp);
				
			}
			System.out.format("\n          ----------\n          %s ",Integer.toBinaryString(c));
			
			tmp = String.format("%8s", Integer.toBinaryString(acc)).replace(' ', '0');
			System.out.format("%s ", tmp);
			
			tmp = String.format("%8s", Integer.toBinaryString(mq)).replace(' ', '0');
			System.out.format("%s\n       >>                     shift right\n", tmp);
			
			c = c << 16;
			acc = acc << 8;
			int shiftTmp = c + acc + mq;
			
			shiftTmp = shiftTmp >>> 1;
			
			/*c = (shiftTmp & 0B10000000000000000) >>> 16;
			acc = (shiftTmp & 0B1111111100000000) >>> 8;
			mq = shiftTmp & 0B11111111;*/
			
			c = (shiftTmp & 0x1000) >>> 16;
			acc = (shiftTmp & 0xff00) >>> 8;
			mq = (shiftTmp & 0xff);
			
			System.out.format("          %s ",Integer.toBinaryString(c));
			
			tmp = String.format("%8s", Integer.toBinaryString(acc)).replace(' ', '0');
			System.out.format("%s ", tmp);
			
			tmp = String.format("%8s", Integer.toBinaryString(mq)).replace(' ', '0');
			System.out.format("%s\n---------------------------------------------------\n", tmp);
			
		}
		
		System.out.println("check:                 binary   decimal");
		
		tmp = String.format("%8s", Integer.toBinaryString(multiplicand)).replace(' ', '0');
		System.out.format("                     %s       %3d\n", tmp, multiplicand);
		
		tmp = String.format("%8s", Integer.toBinaryString(multiplier)).replace(' ', '0');
		System.out.format("           x         %s       %3d\n             ----------------    ------\n", tmp, multiplier);
		
		acc = acc << 8;
		int answer = acc + mq;
		tmp = String.format("%16s", Integer.toBinaryString(answer)).replace(' ', '0');
		System.out.format("             %s     %5d\n", tmp, answer);
		
	}
}
