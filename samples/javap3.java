package primes;

import java.util.LinkedList;
import java.util.List;
import java.util.concurrent.ExecutorService;
import java.util.concurrent.Executors;

class Printer
{
    private static final List<String> listNumbers = new LinkedList(); 
    
    static synchronized void AddResult(int nbToAdd, int threadID)
    {
        listNumbers.add(nbToAdd + " <" + threadID +">");
    }
    
    static void PrintAll()
    {
        for (final String i : listNumbers)
            System.out.println(i);
    }
}

public class PrimeThread implements Runnable
{
    private final int testPrime;
    private final int threadID;
    private boolean isNbPrime;
    
    public PrimeThread(int testPrime, int threadID)
    {
        this.testPrime = testPrime;
        isNbPrime = false;
        this.threadID = threadID;
    }
    
    @Override
    public void run()
    {
        isNbPrime = isPrime(testPrime);
        if (isNbPrime)
            Printer.AddResult(testPrime, threadID);
    }
    
    protected boolean isPrime(int crtNumber)
    {
        if (crtNumber < 4)
            return true;
        
        int nLimit = crtNumber / 2;
        for (int i = 2; i <= nLimit; ++i)
        {
            if (crtNumber % i == 0)
                return false;
        }
        return true;
    }
    
}

public class PrimeSeq
{

    @SuppressWarnings("empty-statement")
    public static void main(String[] args) throws InterruptedException
    {
        int nbThreads = 4;
        int option = 2;

        switch (option)
        {

            case 1:                  // manual thread management
            {
                System.out.println("Running a number of " + nbThreads + " manually managed threads");
                Thread[] thList = new Thread[nbThreads];
                
                for (int i = 1; i < 1000000; i += nbThreads)
                {
                    for (int j = 0; j < nbThreads; ++j)
                    {
                        thList[j] = new Thread(new PrimeThread(i + j, j));
                        thList[j].start();
                    }

                    for (Thread th : thList)
                    {
                        th.join();
                    }
                }            
            }
            break;
            case 2:                  // thread pool cached
            {
                System.out.println("Running a cached pool of " + nbThreads + " threads");
                ExecutorService threadPool = Executors.newFixedThreadPool(nbThreads);
                for (int i = 1; i < 1000000; i += nbThreads)
                {
                    for (int j = 0; j < nbThreads; ++j)
                    {
                        threadPool.submit(new PrimeThread(i + j, j));
                    }
                }
                threadPool.shutdown();
                   
                while (!threadPool.isTerminated())
                {   
                }
            }
            break;
            default:                   // sequential
            {
                System.out.println("Running on a single thread");
                PrimeThread Obj;
                for (int i = 1; i < 1000000; ++i)
                {
                    Obj = new PrimeThread(i, 0);
                    Obj.run();
                }
            }
            break;
        }
        
        Printer.PrintAll();
    }
}
