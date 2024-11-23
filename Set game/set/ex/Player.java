package bguspl.set.ex;

import bguspl.set.Env;

import java.util.*;
import java.util.concurrent.LinkedBlockingQueue;

/**
 * This class manages the players' threads and data
 *
 * @inv id >= 0
 * @inv score >= 0
 */
public class Player implements Runnable {

    /**
     * The game environment object.
     */
    private final Env env;

    /**
     * Game entities.
     */
    private final Table table;
    /**
     * The id of the player (starting from 0).
     */
    public final int id;

    /**
     * The thread representing the current player.
     */
    private Thread playerThread;

    /**
     * The thread of the AI (computer) player (an additional thread used to generate key presses).
     */
    private Thread aiThread;

    /**
     * True iff the player is human (not a computer player).
     */
    private final boolean human;

    /**
     * True iff game should be terminated.
     */
    private volatile boolean terminate;

    /**
     * The current score of the player.
     */
    private volatile int score;

    /**
     * reference to the dealer.
     */
   private final Dealer dealer;                                          //we added
    
    /**
     * The actions queue of the player
     */
    private Queue<Integer> actionQueue;                                //the next action

    /**
     * The actions queue of the player
     */
    private Queue<Integer> actionQueueCard;          //the card which was in slot when the action added 

    /**
     * true if the player is currently waiting
     */
    public volatile boolean isWaiting; 
    /**
     * true if the player is currently need penalty freez
     */
    public volatile boolean needPenalty; 
    /**
     * true if the player is currently need point freez
     */
    public volatile boolean needPoint; 

    private long sec;
  


    /**
     * The class constructor.
     *
     * @param env    - the environment object.
     * @param dealer - the dealer object.
     * @param table  - the table object.
     * @param id     - the id of the player.
     * @param human  - true iff the player is a human player (i.e. input is provided manually, via the keyboard).
     */
    public Player(Env env, Dealer dealer, Table table, int id, boolean human) {
        this.env = env;
        this.table = table;
        this.id = id;
        this.human = human;
        this.dealer = dealer;   
        this.needPenalty = false;
        this.needPoint = false;    
        actionQueue = new LinkedBlockingQueue<Integer>(env.config.featureSize);            //we added
        actionQueueCard = new LinkedBlockingQueue<Integer>(env.config.featureSize);            //we added
        sec = 1000;
    }

    /**
     * The main player thread of each player starts here (main loop for the player thread).
     */
    @Override
    public void run() {
        playerThread = Thread.currentThread();
        env.logger.info("thread " + Thread.currentThread().getName() + " starting.");
        if (!human) createArtificialIntelligence();
        while (!terminate) {
            // TODO implement main player loop
            if(needPenalty){
                penalty();
            }
            if(needPoint){
                point();
            }
            if(!isWaiting){
                if(human){
                    try {
                        synchronized (this){
                            this.wait();
                        }
                        } catch (InterruptedException ignored) {} 
                }
                action();  
            }
            else if (isWaiting) {
                try {
                    synchronized (this){
                        wait();
                    }
                    } catch (InterruptedException ignored) {} 
            }
        }
        if (!human) try { aiThread.join(); } catch (InterruptedException ignored) {}
        env.logger.info("thread " + Thread.currentThread().getName() + " terminated.");
    }

    /**
     * Creates an additional thread for an AI (computer) player. The main loop of this thread repeatedly generates
     * key presses. If the queue of key presses is full, the thread waits until it is not full.
     */
    private void createArtificialIntelligence() {
        // note: this is a very, very smart AI (!)
        aiThread = new Thread(() -> {           //for bonuse mabye should check amount of ai Thread
            env.logger.info("thread " + Thread.currentThread().getName() + " starting.");
            while (!terminate) {
                while(actionQueue.size() < env.config.featureSize & !isWaiting){
                    Random x = new Random();
                    int randomSlot = x.nextInt(env.config.tableSize);
                    keyPressed(randomSlot);
                    // try {
                    //     aiThread.sleep(10);
                    // } catch (InterruptedException ignored) {}
                }
            }
            env.logger.info("thread " + Thread.currentThread().getName() + " terminated.");
        }, "computer-" + id);
        aiThread.start();
    }

    /**
     * Called when the game should be terminated.
     */
    public void terminate() {
        isWaiting = false;
        terminate = true;
        notifyPlayer();
        try {
            playerThread.join();
        } catch (InterruptedException ignored){} 
    }

    /**
     * This method is called when a key is pressed.
     *
     * @param slot - the slot corresponding to the key pressed.
     */
    public void keyPressed(int slot) {
        if(actionQueue.size() < env.config.featureSize && !isWaiting && table.getSlotToCard(slot) != -1){   //if action is legal
            isWaiting = false;
            actionQueueCard.add(table.getSlotToCard(slot));             //the current card while we add the action
            actionQueue.add(slot);                                      //next action
            if (human) {
                notifyPlayer();
            }
        }
    }

    /**
     * Award a point to a player and perform other related actions.
     *
     * @post - the player's score is increased by 1.
     * @post - the player's score is updated in the ui.
     */
    public void  point() { 
        int ignored = table.countCards();
        score++;
        env.ui.setScore(id, score); 
        playerFreeze(env.config.pointFreezeMillis);
        needPoint = false;
    }

    /**
     * Penalize a player and perform other related actions.
     */
    public void penalty() {
        playerFreeze(env.config.penaltyFreezeMillis);
        needPenalty = false;
    }

    /**
     * This method send player's thread to sleap for milliesForSleep time.
     *
     * @param milliesForSleep - amount of time player will sleep.
     */
    private void playerFreeze(long milliesForSleep){ //freezes the player. used in case of point or penalty.   //meytav
        isWaiting = true; 
        while (milliesForSleep > 0) {
            env.ui.setFreeze(id, milliesForSleep); // shows on the screen the time left to be freezed
            try{this.playerThread.sleep(sec);} catch(InterruptedException e){}; // sleep for one second// Meytav isnt sure if has to add something in the {}.
            milliesForSleep = milliesForSleep - 1000; 
        }
        env.ui.setFreeze(id,0);
        isWaiting = false;  
    }


    /**
     * get player's score
     */
    public int score(){
        return score;
    }

    /**
     * true if player have token in slot
     *
     * @param slot - the slot to check.
     */
    public boolean haveTokenInSlot(int slot){ 
        return table.PlayerHasTokenInSlot(id,slot);
    }

    /**
     * get player's next action and commit
     */
    public void action(){
        if(!actionQueue.isEmpty()){
            boolean startWithTwo = table.numOfTokensForPlayer(id) == env.config.featureSize -1;
            int card = actionQueueCard.poll();
            int slot = actionQueue.poll();
            table.acquireSlotLock(slot);
            if(!isWaiting && table.getSlotToCard(slot) == card){
                table.placeToken(id, slot);
            } 
            boolean finishWithThree = table.numOfTokensForPlayer(id) == env.config.featureSize;
            if(startWithTwo & finishWithThree){
                isWaiting = true;
            }
            table.releaseSlotLock(slot);
        }
    }
    
    /**
     * change player isWaiting to false
     */
    public void wake(){
        isWaiting = false;
    }

    public void waiting(){
        isWaiting = false;
    }


  public synchronized void notifyPlayer(){
    notifyAll();
  }

}


