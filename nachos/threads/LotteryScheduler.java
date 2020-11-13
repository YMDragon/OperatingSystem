package nachos.threads;

import nachos.machine.*;

import java.util.TreeSet;
import java.util.HashSet;
import java.util.Iterator;

/**
 * A scheduler that chooses threads using a lottery.
 *
 * <p>
 * A lottery scheduler associates a number of tickets with each thread. When a
 * thread needs to be dequeued, a random lottery is held, among all the tickets
 * of all the threads waiting to be dequeued. The thread that holds the winning
 * ticket is chosen.
 *
 * <p>
 * Note that a lottery scheduler must be able to handle a lot of tickets
 * (sometimes billions), so it is not acceptable to maintain state for every
 * ticket.
 *
 * <p>
 * A lottery scheduler must partially solve the priority inversion problem; in
 * particular, tickets must be transferred through locks, and through joins.
 * Unlike a priority scheduler, these tickets add (as opposed to just taking the
 * maximum).
 */
public class LotteryScheduler extends PriorityScheduler {
    /**
     * Allocate a new lottery scheduler.
     */
    public LotteryScheduler() {
    }

    /**
     * Allocate a new lottery thread queue.
     *
     * @param transferPriority <tt>true</tt> if this queue should transfer tickets
     *                         from waiting threads to the owning thread.
     * @return a new lottery thread queue.
     */
    @Override
    public ThreadQueue newThreadQueue(boolean transferPriority) {
        return new LotteryQueue(transferPriority);
    }

    @Override
    protected ThreadState getThreadState(KThread thread) {
        if (thread.schedulingState == null)
            thread.schedulingState = new LotteryThreadState(thread);

        return (ThreadState) thread.schedulingState;
    }

    protected class LotteryQueue extends PriorityQueue {
        LotteryQueue(boolean transferPriority) {
            super(transferPriority);
        }

        @Override
        protected ThreadState pickNextThread() {
            int totalTicket = 0;
            for (ThreadState threadState : waitingQueue)
                totalTicket += threadState.getEffectivePriority();
            int chosenTicket = Random.nextInt(totalTicket);
            for (ThreadState threadState : waitingQueue)
                if (chosenTicket < threadState.getEffectivePriority())
                    return threadState;
            return null;
        }

        @Override
        public void updateEffectivePriority() {
            int oldEffectivePriority = effectivePriority;
            effectivePriority = 0;
            for (ThreadState threadState : waitingQueue)
                effectivePriority += threadState.getEffectivePriority();
            if (oldEffectivePriority != effectivePriority && holdThread != null)
                holdThread.updateEffectivePriority();
        }
    }

    protected class LotteryThreadState extends ThreadState {
        public LotteryThreadState(KThread thread) {
            super(thread);
        }

        @Override
        protected void updateEffectivePriority() {
            int oldEffectivePriority = effectivePriority;
            effectivePriority = priority;
            for (PriorityQueue threadQueue : holdingQueue)
                effectivePriority += threadQueue.getEffectivePriority();

            if (oldEffectivePriority != effectivePriority)
                for (PriorityQueue threadQueue : waitingQueue)
                    threadQueue.updateEffectivePriority();
        }
    }
}
