package bguspl.set.ex;

import bguspl.set.Env;

import java.util.ArrayList;
import java.util.Arrays;
import java.util.List;
import java.util.Objects;
import java.util.stream.Collectors;
import java.util.concurrent.BlockingQueue;
import java.util.concurrent.LinkedBlockingQueue;
import java.util.concurrent.Semaphore;


/**
 * This class contains the data that is visible to the player.
 *
 * @inv slotToCard[x] == y iff cardToSlot[y] == x
 */
public class Table {

    /**
     * The game environment object.
     */
    private final Env env;

    /**
     * Mapping between a slot and the card placed in it (null if none).
     */
    protected final Integer[] slotToCard; // card per slot (if any)

    /**
     * Mapping between a card and the slot it is in (null if none).
     */
    protected final Integer[] cardToSlot; // slot per card (if any)

    /**
     * The game current token place.
     */
    private volatile boolean[][][] playersTokens;

    /**
     * hold the player id which hold the next action.
     */
    private BlockingQueue<Integer> nextPlayerToCheck;               //gal added

    /**
     * true if player set need to be check.
     */
    // public boolean[] playerToCheck; 


    private Semaphore[] slotLock;


    /**
     * hold the player id which hold the next action.
     */

    /**
     * Constructor for testing.
     *
     * @param env        - the game environment objects.
     * @param slotToCard - mapping between a slot and the card placed in it (null if none).
     * @param cardToSlot - mapping between a card and the slot it is in (null if none).
     */
    public Table(Env env, Integer[] slotToCard, Integer[] cardToSlot) {
        this.env = env;
        this.slotToCard = slotToCard;
        this.cardToSlot = cardToSlot;
        playersTokens = new boolean[env.config.players][env.config.rows][env.config.columns];
        nextPlayerToCheck = new LinkedBlockingQueue<Integer>();            //gal added
        slotLock = new Semaphore[slotToCard.length];
        for(int i = 0; i < slotToCard.length; i++){
            slotLock[i] = new Semaphore(1, true);
        }
    }

    /**
     * Constructor for actual usage.
     *
     * @param env - the game environment objects.
     */
    public Table(Env env) {
        this(env, new Integer[env.config.tableSize], new Integer[env.config.deckSize]);
    }

    /**
     * This method prints all possible legal sets of cards that are currently on the table.
     */
    public void hints() {
        List<Integer> deck = Arrays.stream(slotToCard).filter(Objects::nonNull).collect(Collectors.toList());
        env.util.findSets(deck, Integer.MAX_VALUE).forEach(set -> {
            StringBuilder sb = new StringBuilder().append("Hint: Set found: ");
            List<Integer> slots = Arrays.stream(set).mapToObj(card -> cardToSlot[card]).sorted().collect(Collectors.toList());
            int[][] features = env.util.cardsToFeatures(set);
            System.out.println(sb.append("slots: ").append(slots).append(" features: ").append(Arrays.deepToString(features)));
        });
    }

    /**
     * Count the number of cards currently on the table.
     *
     * @return - the number of cards on the table.
     */
    public int countCards(){
        int cards = 0;
        for (int i = 0; i < slotToCard.length; i++){
            if (slotToCard[i] != null)
                ++cards;
        }
        return cards;
    }

    /**
     * Places a card on the table in a grid slot.
     * @param card - the card id to place in the slot.
     * @param slot - the slot in which the card should be placed.
     *
     * @post - the card placed is on the table, in the assigned slot.
     */
    public void placeCard(int card, int slot) {
        try {
            Thread.sleep(env.config.tableDelayMillis);
        } catch (InterruptedException ignored) {}
        cardToSlot[card] = slot;
        slotToCard[slot] = card;
        env.ui.placeCard(card, slot);
        }
    
    /**
     * Removes a card from a grid slot on the table.
     * @param slot - the slot from which to remove the card.
     */
    public void removeCard(int slot) {
        try {
            Thread.sleep(env.config.tableDelayMillis);
        } catch (InterruptedException ignored) {}
        cardToSlot[slotToCard[slot]] = null;
        slotToCard[slot] = null;
        removeTokensFromSlot(slot);
        env.ui.removeCard(slot);
    }
    
    /**
     * Places a player token on a grid slot.
     * @param player - the player the token belongs to.
     * @param slot   - the slot on which to place the token.
     */
    public void placeToken(int player, int slot) {
        if(PlayerHasTokenInSlot(player, slot)){
            removeToken(player, slot);
        } 
        else if(numOfTokensForPlayer(player) < env.config.featureSize & slotToCard[slot] != null){
            env.ui.placeToken(player, slot);
            int row = slot / env.config.columns;
            int column = slot % env.config.columns;
            playersTokens[player][row][column] = true;
            if(numOfTokensForPlayer(player) == env.config.featureSize){
                setNextPlayerToCheck(player);
            }
        }
    }

    /**
     * Removes a token of a player from a grid slot.
     * @param player - the player the token belongs to.
     * @param slot   - the slot from which to remove the token.
     * @return       - true iff a token was successfully removed.
     */
    public boolean removeToken(int player, int slot) {
        int row = slot / env.config.columns;
        int column = slot % env.config.columns;
        if(playersTokens[player][row][column] & slotToCard[slot] != null){
            env.ui.removeToken(player, slot);
            playersTokens[player][row][column] = false;
            return true;
        }
        return false;
    }

    /**
     * remove all player's tokens from slot.
     * @param slot - the slot from which to check.
     */
    public void removeTokensFromSlot(int slot){
        int row = slot / env.config.columns;
        int column = slot % env.config.columns;
        for(int i = 0; i < env.config.players; i++){        //remove all tokens on this slot
            if(playersTokens[i][row][column])
            playersTokens[i][row][column] = false;; 
        } 
        env.ui.removeTokens(slot);    
    }

    /**
     * true if player have token in slot.
     * @param slot - the slot from which to check.
     * @param player - the player that wich we check.
     */
    public  boolean PlayerHasTokenInSlot(int player, int slot){
        int row = slot / env.config.columns;
        int column = slot % env.config.columns;
        return playersTokens[player][row][column];
    }

    /**
     * return boolean aray of PlayersTokens
     */
     public boolean[][][] getPlayersTokens(){
       return playersTokens;
    }

    /**
     * add action to the action queue
     */
    public void setNextPlayerToCheck(int id){
        nextPlayerToCheck.add(id);
    }

    /**
     * return the card currently in the slot.
     * or -1 if there is no card in the slot
     * @param slot - the slot from which to check.
     */
    public int getSlotToCard(int slot){
        if(slotToCard[slot] == null){
            return -1;
        }
        return slotToCard[slot];
    }

    /**
     * return the slot witch currently the card are place in.
     * or -1 if the card not on the table
     * @param card - the slot from which to check.
     */
    public int getCardToSlot(int card){
        if(cardToSlot[card] == null){
            return -1;
        }
        return cardToSlot[card];
    }

    /**
     * return all the card that currentlly on the table.
     */    
    public Integer[] getAraaySlotToCard(){
        return slotToCard;
    }

    /**
     * true if there is a set on the table.
     */ 
    public boolean haveSetOnTable(){
        List<Integer> cardsOnTable = new ArrayList<Integer>(countCards());
        for(int i = 0; i < slotToCard.length; i++){
            if(slotToCard[i] != null){
                cardsOnTable.add(slotToCard[i]);
            }
        }
        return (env.util.findSets(cardsOnTable, 1).size() != 0);
    }

    /**
     * return the num of token player curentlly have on the table.
     * @param player - the player from which to check.
     */
    public int numOfTokensForPlayer(int player){
        int output = 0;
        for (int row = env.config.rows - 1; row >= 0; row-- ){
            for (int column = env.config.columns -1; column >= 0; column--){
                if (playersTokens[player][row][column] == true)
                    output++;
          }
        }
        return output;
    }

    /**
     * return the cards that the player place token on.
     * @param player - the player from which to check.
     */
    public int[] getPlayersCardsWithToken(int player){
        int[] output = new int[env.config.featureSize];
        int i = 0;
        for (int row = 0; row < env.config.rows;  row++){
            for (int column = 0; column < env.config.columns; column++){
                if (playersTokens[player][row][column] == true){
                    output[i] = getSlotToCard((row)*env.config.columns + column);
                    i++;
                }
            }
        }
        return output;
    }

    /**
     * return which player need to action next
     */
    public int getNextPlayerToCheck(){
        return nextPlayerToCheck.poll();
    }

    /**
     * true if there action that wait.
     */
    public boolean thereIsNextPlayerToCheck(){
        return !nextPlayerToCheck.isEmpty();
    }

    public void acquireSlotLock(int slot){
        try {
            slotLock[slot].acquire();
        } catch (InterruptedException e) {
            e.printStackTrace();
        }
    }

    public void acquireAllSlotLock(){
        for(int i = 0; i < slotToCard.length; i++){
            acquireSlotLock(i);
        }
    }

    public void releaseSlotLock(int slot){
        slotLock[slot].release();
    }

    public void releaseAllSlotLock(){
        for(int i = 0; i < slotToCard.length; i++){
            releaseSlotLock(i);
        }
    }

}
