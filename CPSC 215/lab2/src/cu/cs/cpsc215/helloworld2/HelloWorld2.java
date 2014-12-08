package cu.cs.cpsc215.helloworld2;

public class HelloWorld2 {

	/**
	 * @param args
	 */
	public static void main(String[] args) {
		System.out.println("Hello World!");
		for( int i =0; i<args.length; i++) {
			System.out.println(args[i]);
		}
		System.out.println(args.length);
	}

}
