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

        numSpeak += 1;
        wordQueue.offer(word);

        if(numListen > 0)
            waitListen.wake();
        else{
            while(numListen == 0)
                waitSpeak.sleep();
        }

        numListen -= 1;

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

        numListen += 1;

        if(numSpeak > 0)
            waitSpeak.wake();
        else{
            while(numSpeak == 0)
                waitListen.sleep();
        }
        
        int word = wordQueue.remove();
        numSpeak -= 1;

        lock.release();
        
        return word;
    }
    private Condition2 waitSpeak, waitListen;
    private Lock lock = new Lock();
    private int numSpeak, numListen;
    private Queue<Integer> wordQueue = new LinkedList<Integer>();
}
