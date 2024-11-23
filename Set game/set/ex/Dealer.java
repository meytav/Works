package bguspl.set.ex;

import bguspl.set.Env;
import java.util.List;
import java.util.stream.Collectors;
import java.util.stream.IntStream;
import java.util.ArrayList;
import java.util.Collections;

/**
 * This class manages the dealer's threads and data
 */
public class Dealer implements Runnable {

    /**
     * The game environment object.
     */
    private final Env env;

    /**
     * Game entities.
     */
    private final Table table;
    private final Player[] players;

    /**
     * The list of card ids that are left in the dealer's deck.
     */
    private final List<Integer> deck;

    /**
     * True iff game should be terminated.
     */
    private volatile boolean terminate;

    /**
     * The time when the dealer needs to reshuffle the deck due to turn timeout.
     */
    private long reshuffleTime;

    private long timeWhenReset;

    private int[] cardsToRemove; 

    private boolean[] alredyUse;


    

    public Dealer(Env env, Table table, Player[] players) {
        this.env = env;
        this.table = table;
        this.players = players;
        deck = IntStream.range(0, env.config.deckSize).boxed().collect(Collectors.toList());
        timeWhenReset = System.currentTimeMillis();
        reshuffleTime =  timeWhenReset + env.config.turnTimeoutMillis;
        cardsToRemove = new int[env.config.featureSize];
        for(int i = 0; i < cardsToRemove.length; i++){
            cardsToRemove[i] = -1;
        }
        this.alredyUse = new boolean[table.cardToSlot.length];
        for(int i = 0; i < table.cardToSlot.length; i++){
            alredyUse[i] = false;
        }
    }

    /**
     * The dealer thread starts here (main loop for the dealer thread).
     */
    @Override
    public void run() {
        env.logger.info("thread " + Thread.currentThread().getName() + " starting.");
        //create players threads
        for (Player player : players) {
            Thread playerThread = new Thread(player, "Player " + player.id);
            player.waiting();
            playerThread.start();
        }
        while (!shouldFinish()) {
            timeWhenReset = System.currentTimeMillis();
            reshuffleTime =  timeWhenReset + env.config.turnTimeoutMillis;
            updateTimerDisplay(false);
            Collections.shuffle(deck);
            table.acquireAllSlotLock();
            placeCardsOnTable();
            table.releaseAllSlotLock();
            for(int i = 0; i < players.length; i ++){
                players[i].wake();
                players[i].notifyPlayer();
            }
            if(env.config.hints){
                table.hints();
            }
            timerLoop();
            updateTimerDisplay(false);
        }
        removeAllCardsFromTable();
        announceWinners();
        terminate();
        try {
            Thread.sleep(env.config.endGamePauseMillies);
        } catch (InterruptedException e) {
            e.printStackTrace();
        }
        env.logger.info("thread " + Thread.currentThread().getName() + " terminated.");
    }

    /**
     * The inner loop of the dealer thread that runs as long as the countdown did not time out.
     */
    private void timerLoop() {
        timeWhenReset = System.currentTimeMillis();
        reshuffleTime = timeWhenReset + env.config.turnTimeoutMillis;
        while (!shouldFinish() && System.currentTimeMillis() < reshuffleTime) {
            sleepUntilWokenOrTimeout();
            updateTimerDisplay(false);
            while (!shouldFinish() && table.thereIsNextPlayerToCheck()) {
                int playerId = table.getNextPlayerToCheck();
                if(table.numOfTokensForPlayer(playerId) == env.config.featureSize){
                    testSet(table.getPlayersCardsWithToken(playerId),playerId);
                }
                players[playerId].wake();
                players[playerId].notifyPlayer();
            }
        }
        updateTimerDisplay(true);
        if(!terminate){
            table.acquireAllSlotLock();
            removeAllCardsFromTable();
            placeCardsOnTable();
            table.releaseAllSlotLock();
        }

    }

    /**
     * Called when the game should be terminated.
     */
    public void terminate() {
        terminate = true;
        for (int i = players.length - 1; i >= 0; i--){
            players[i].terminate();
        }
    }

    /**
     * Check if the game should be terminated or the game end conditions are met.
     *
     * @return true iff the game should be finished.
     */
    private boolean shouldFinish() {
        if (env.util.findSets(deck, 1).size() == 0 && !table.haveSetOnTable()) {
            terminate();
        }
        return (terminate || env.util.findSets(deck, 1).size() == 0 && !table.haveSetOnTable());
    }


    /**
     * Returns all the cards from the table to the deck.
     */
    private void removeAllCardsFromTable() {
        for(int i = 0; i < table.slotToCard.length; i++){
            if(table.slotToCard[i] != null){
                deck.add(table.slotToCard[i]);
                sleepUntilWokenOrTimeout();
                table.removeCard(i);
            }
        }
        Collections.shuffle(deck);
    }
    
    
    /**
     * Checks cards should be removed from the table and removes them.
     */
    private void removeCardsFromTable(){
        for(int i = 0; i < cardsToRemove.length; i++){
            sleepUntilWokenOrTimeout();
            table.removeCard(cardsToRemove[i]);
            cardsToRemove[i] = -1;
        }
    }



    /**
     * Check if any cards can be removed from the deck and placed on the table.
     */
    private void placeCardsOnTable() {
        // TODO implement
        int slot = 0;
        int NumOfCardsToPlace = table.slotToCard.length - table.countCards();
        Integer[] tableCards = table.getAraaySlotToCard();
        for(int i = 0; i < tableCards.length; i++){
            if(tableCards[i] == null && deck.size() > 0 & NumOfCardsToPlace > 0){
                sleepUntilWokenOrTimeout(); 
                table.placeCard(deck.remove(0), slot);
                NumOfCardsToPlace--;
            }
            slot++;
        }
    }


    /**
     * Sleep for a fixed amount of time or until the thread is awakened for some purpose.
     */
    private void sleepUntilWokenOrTimeout() {
        try {
            Thread.sleep(env.config.tableDelayMillis);
        } catch (InterruptedException ignored) {}
    }

    /**
     * Reset and/or update the countdown and the countdown display.
     */
    private void updateTimerDisplay(boolean reset) {
        // TODO implement
        if (env.config.turnTimeoutMillis < 0) {
            return;                                             // Display nothing
        } 
        else if (env.config.turnTimeoutMillis == 0) {
            long currentTime = System.currentTimeMillis();     // display timer since last action
            long elapsedTime = currentTime - timeWhenReset;
            env.ui.setElapsed(elapsedTime);
        }
        else {
            if(reset){
                timeWhenReset = System.currentTimeMillis();
                reshuffleTime = timeWhenReset + env.config.turnTimeoutMillis;
                env.ui.setCountdown(env.config.turnTimeoutMillis, false);
            }
            else{
                long elapsedTime = System.currentTimeMillis() - timeWhenReset;
                long remainingTime =  env.config.turnTimeoutMillis - elapsedTime;
                boolean warn = false;
                if(remainingTime < env.config.turnTimeoutWarningMillis)
                    warn = true;
                env.ui.setCountdown(remainingTime, warn);
            }
        }
    }



    void testSet(int[] cards, int player){
        int slots[] = new int[cards.length];
        for(int i = 0; i < cards.length; i++){
            if(table.cardToSlot[cards[i]] != null){
                slots[i] = table.cardToSlot[cards[i]];
                table.acquireSlotLock(slots[i]);
            }
        }
        if(!env.util.testSet(cards)){
                players[player].needPenalty = true;
        }
        else{
            updateTimerDisplay(true);
            for(int i = 0; i < cards.length; i++){
                cardsToRemove[i] = slots[i];
            }
            removeCardsFromTable();
            placeCardsOnTable();
            updateTimerDisplay(true);
            players[player].needPoint = true;
        }
        for(int i = 0; i < slots.length; i++){
            table.releaseSlotLock(slots[i]);
        }
    }

    /**
     * Check who is/are the winner/s and displays them.
     */
    private void announceWinners() {
        // TODO implement
        List<Integer> winnersIds = new ArrayList<>();
        int maxScore = 0;                                       //find the maximum score
        for (int i = 0; i < players.length; i++) { 
            if (players[i].score() > maxScore) {
                maxScore = players[i].score();
            }
        }
        for(Player player: players){                             //find all players with maxScore
            if (player.score() == maxScore){
                winnersIds.add(player.id);
            }  
        }
        int[] arr = new int[winnersIds.size()];
        for(int i = 0; i < arr.length; i++){
            arr[i] = winnersIds.get(i);
        }
         env.ui.announceWinner(arr); 
    }

    public Player[] getPlayers(){
        return players;
    }

    
}

