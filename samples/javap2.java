class TestClass 
{
   public static void main(String[] args) 
   {
       int a = 5;
	   
	   '\'';

       int[] arr = new int[8];
       char b = 'b';
	   
	   double x = 3.14159;
	   double y = 3.14159e-10;   

        // this line will be ignored
        a / 5;
		
		double z = x+++y;
		
		int _xi = 1;
		int _zi = (_xi >>>= 1) << 1;
		
		int m = 15, n = 10;
		m = m ^ n;
		n = m ^ n;
		m = m ^ n;
		
		// This will produce an error
	    1.e
		
		"I'm a strange \"string\" literal here"
		
		boolean _val = true
		if (_val == true)
			System.out.println("YES");

        if (a < b) {
            a++;
            return false;
        } else if (a <= b) {
            b+=7;
            return true;
        }

       return null /* The value of pi is p=3.14159 */;
   }
}
