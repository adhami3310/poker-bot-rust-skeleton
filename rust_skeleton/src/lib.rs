mod ctypes;
use ctypes::*;

// Called whenever a new round starts
#[no_mangle]
pub extern "C" fn handle_new_round(game_info: GameInfo, round_info: RoundInfo) {
    println!("Round initialized!");
    println!("Game info: {:?}", game_info);
    println!("Round info: {:?}", round_info);
}

// Called whenever a round begins with a new deal of cards
//
// Returns the action that will be taken per turn
// this is where most of the meat of the code should end up calling into
#[no_mangle]
pub extern "C" fn action(game_info: GameInfo, round_info: RoundInfo) -> Action {
    Action {
        action_type: ActionType::Fold,
        amount: 0,
    }
}

// // Called whenever a round is over with results
// // and available information about the round
// #[no_mangle]
// pub extern "C" fn handle_round_over(game_info: GameInfo, round_over_info : RoundOverInfo) {
//     println!("Round over!");
//     println!("Game info: {:?}", game_info);
//     println!("Round over info: {:?}", round_over_info);

// }
