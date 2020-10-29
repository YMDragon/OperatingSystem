package nachos.threads;

import nachos.machine.*;

import java.util.LinkedList;
import java.util.Queue;

/**
 * A <i>communicator</i> allows threads to synchronously exchange 32-bit
 * messages. Multiple threads can be waiting to <i>speak</i>, and multiple
 * threads can be waiting to <i>listen</i>. But there should never be a time
 * when both a speaker and a listener are waiting, because the two threads can
 * be paired off at this point.
 */
public class Communicator {
    /**
     * Allocate a new communicator.
     */
    public Communicator() {
        waitSpeak = new Condition2(lock);
        waitListen = new Condition2(lock);
        hasWord = false;
    }

    /**
     * Wait for a thread to listen through this communicator, and then transfer
     * <i>word</i> to the listener.
     *
     * <p>
     * Does not return until this thread is paired up with a listening thread.
     * Exactly one listener should receive <i>word</i>.
     *
     * @param word the integer to transfer.
     */
    public void speak(int word) {
        lock.acquire();

        while(hasWord)
            waitSpeak.sleep();

        hasWord = true;
        wordTrans = word;
        waitListen.wake();

        lock.release();
    }

    /**
     * Wait for a thread to speak through this communicator, and then return the
     * <i>word</i> that thread passed to <tt>speak()</tt>.
     *
     * @return the integer transferred.
     */
    public int listen() {
        lock.acquire();

        while(!hasWord)
            waitListen.sleep();
        
        int word = wordTrans;
        hasWord = false;
        waitSpeak.wake();
        
        lock.release();
        
        return word;
    }
    public static void selfTest(){
        Communicator comm = new Communicator();
        Runnable sp = new Runnable(){
            public void run(){
                int num = Lib.random(100);
                System.out.println("speak " + num);
                comm.speak(num);
            }
        };
        Runnable li = new Runnable(){
            public void run(){
                System.out.println("listen " + comm.listen());
            }
        };
        KThread t1 = new KThread(sp);
        t1.fork();
        KThread t2 = new KThread(li);
        t2.fork();
        KThread t3 = new KThread(li);
        t3.fork();
        KThread t4 = new KThread(sp);
        t4.fork();
        for(int i = 0; i < 4; i ++){
            KThread.yield();
        }
    }

    private Condition2 waitSpeak, waitListen;
    private Lock lock = new Lock();
    private int wordTrans;
    private boolean hasWord;
}
