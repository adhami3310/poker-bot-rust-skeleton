use std::ffi::{c_double, c_int};

#[derive(Debug, Clone, Copy)]
#[repr(C)]
pub struct GameInfo {
    pub bankroll: c_int,
    pub game_clock: c_double,
    pub round_num: c_int,
}

#[derive(Debug, Clone, Copy)]
#[repr(C)]
pub enum ActionType {
    Fold,
    Call,
    Check,
    Raise,
    Bid,
}

#[derive(Debug, Clone, Copy)]
#[repr(C)]
pub struct Action {
    pub action_type: ActionType,
    pub amount: c_int,
}

#[derive(Debug, Clone, Copy)]
#[repr(C)]
pub struct RoundInfo {
    pub active: c_int,
    pub street: c_int,
    pub auction: bool,
    pub bids: [c_int; 2],
    pub pips: [c_int; 2],
    pub stacks: [c_int; 2],
    pub hands: [[c_int; 3]; 2],
    pub deck: [c_int; 5],
    pub little_folded: c_int,
    pub total_little_rounds: c_int,
}