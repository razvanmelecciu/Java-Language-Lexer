package com.myPackage;

public class TestClass implements Serializable
{
   public static void main(String[] args) 
   {
       int a = 5;
	   int b = 123l;
	   int c = 0xA1L, d = 0123l, e = 1234L;
	   int _1 = 0XFFL; int $2 = 0X00L;
	   
	   char $ch = '\'';
	   char _ch = 'b';
     
     long __$ = 0x0l;
	   
       int[] arr = new int[8];
	   
	   double x = 3.14159;
	   double y = 3.14159e-10;  
	   double z = .0;	   
	   double u = .0e-1f, v = .0e1, w = .0e1f;
     
     /*
     " This is a weird 
     comment"*//*
     */
	   
       // this line will be ignored
       a / 5.123E+05F;
       
     a /= 5.123E+05F;  
		
	   double z = (x+++y)*=15.1E0F;
		
	   int _xi = 1;
	   int _zi = (_xi >>>= 1) << 1;
		
		int m = 15, n = 10;
		m = m ^ n;
		n = m ^ n;
		m = m ^ n;
		
		System.out.println("The value of pi is p=3.14159");
		
		// This will produce an error
	    1.efloat xyz = 15.;
		
		"I'm a strange \"string\" literal here"
		
		boolean _val = true;
		if (_val == true)
			System.out.println("YES");
		
		// This will also produce an error
		double _$k = 15.1EF;

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
