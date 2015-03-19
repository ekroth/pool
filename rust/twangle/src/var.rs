use std::ops::Range;

pub type VarValue = isize;

#[derive(Show, Copy, PartialEq)]
pub struct Var {
    pub name: &'static str,
    pub range: Range<VarValue>,
}
