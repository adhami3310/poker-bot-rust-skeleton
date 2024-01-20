#include <skeleton/actions.h>
#include <skeleton/constants.h>
#include <skeleton/runner.h>
#include <skeleton/states.h>
#include <iostream>

#include <random>

using namespace pokerbots::skeleton;

int cardToNumber(std::string card)
{
  if (card.length() == 2)
  {
    int offset = 0;
    if (card[1] == 'h')
    {
      offset = 1;
    }
    else if (card[1] == 'd')
    {
      offset = 2;
    }
    else if (card[1] == 'c')
    {
      offset = 3;
    }
    int value = 0;
    if ('2' <= card[0] && card[0] <= '9')
    {
      value = (card[0] - '2') + 1;
    }
    else if (card[0] == 'T')
    {
      value = 9;
    }
    else if (card[0] == 'J')
    {
      value = 10;
    }
    else if (card[0] == 'Q')
    {
      value = 11;
    }
    else if (card[0] == 'K')
    {
      value = 12;
    }
    return value + 13 * offset;
  }
  else
  {
    return -1;
  }
}

extern "C"
{

  struct RoundInfo
  {
    int active;
    int street;
    bool auction;
    int bids[2];
    int pips[2];
    int stacks[2];
    int hands[2][3];
    int deck[5];
    int little_folded;
    int total_little_rounds;

    RoundInfo(int active, int street, bool auction, std::array<std::optional<int>, 2> bids, std::array<int, 2> pips, std::array<int, 2> stacks, std::array<std::array<std::string, 3>, 2> hands, std::array<std::string, 5> deck)
    {
      this->active = active;
      this->street = street;
      this->auction = auction;
      this->bids[0] = bids[0].value_or(-1);
      this->bids[1] = bids[1].value_or(-1);
      this->pips[0] = pips[0];
      this->pips[1] = pips[1];
      this->stacks[0] = stacks[0];
      this->stacks[1] = stacks[1];
      for (int i = 0; i < 2; i++)
      {
        for (int j = 0; j < 3; j++)
        {
          this->hands[i][j] = cardToNumber(hands[i][j]);
        }
      }
      for (int i = 0; i < 5; i++)
      {
        this->deck[i] = cardToNumber(deck[i]);
      }
    }
  };

  // Define all Rust FFI functions here with appropiate types
  void handle_new_round(GameInfo gameState, RoundInfo roundInfo);
  Action action(GameInfo GameState, RoundInfo roundInfo);
}

struct Bot
{
  /*
    Called when a new round starts. Called NUM_ROUNDS times.

    @param gameState The GameState object.
    @param roundState The RoundState object.
    @param active Your player's index.
  */
  void handleNewRound(GameInfoPtr gameState, RoundStatePtr roundState, int active)
  {
    // int myBankroll = gameState->bankroll;  // the total number of chips you've gained or lost from the beginning of the game to the start of this round
    // float gameClock = gameState->gameClock;  // the total number of seconds your bot has left to play this game
    // int roundNum = gameState->roundNum;  // the round number from 1 to State.NUM_ROUNDS
    // auto myCards = roundState->hands[active];  // your cards
    // bool bigBlind = (active == 1);  // true if you are the big blind
    auto roundInfo = new RoundInfo(roundState->button, roundState->street, roundState->auction, roundState->bids, roundState->pips, roundState->stacks, roundState->hands, roundState->deck);
    handle_new_round(*gameState.get(), *roundInfo);
    delete roundInfo;
  }

  /*
    Called when a round ends. Called NUM_ROUNDS times.

    @param gameState The GameState object.
    @param terminalState The TerminalState object.
    @param active Your player's index.
  */
  void handleRoundOver(GameInfoPtr gameState, TerminalStatePtr terminalState, int active)
  {
    // std::cout << active << " " << folded << " " << total_little << std::endl;
    // int myDelta = terminalState->deltas[active];  // your bankroll change from this round
    // auto myCards = previousState->hands[active];  // your cards
    // auto oppCards = previousState->hands[1-active];  // opponent's cards or "" if not revealed
  }

  /*
    Where the magic happens - your code should implement this function.
    Called any time the engine needs an action from your bot.

    @param gameState The GameState object.
    @param roundState The RoundState object.
    @param active Your player's index.
    @return Your action.
  */
  Action getAction(GameInfoPtr gameState, RoundStatePtr roundState, int active)
  {
    auto roundInfo = new RoundInfo(active, roundState->street, roundState->auction, roundState->bids, roundState->pips, roundState->stacks, roundState->hands, roundState->deck);
    Action actionDo = action(*gameState.get(), *roundInfo);
    delete roundInfo;
    return actionDo;
  }
};

/*
  Main program for running a C++ pokerbot.
*/
int main(int argc, char *argv[])
{
  auto [host, port] = parseArgs(argc, argv);
  runBot<Bot>(host, port);
  return 0;
}
