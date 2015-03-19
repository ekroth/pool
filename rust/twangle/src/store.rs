use std::collections::HashMap;

use constraint::Constraint;
use solver::Solver;
use var::VarValue;

#[derive(Show)]
pub struct Store {
    constraints: Vec<Constraint>,
}

#[derive(Clone)]
pub struct Pass {
    pub vars: HashMap<&'static str, VarValue>,
}

impl Pass {
    pub fn new(vars: HashMap<&'static str, VarValue>) -> Pass {
        Pass { vars: vars }
    }
}

impl Store {
    pub fn new() -> Store {
        Store {
            constraints: vec!(),
        }
    }

    pub fn impose(&mut self, c: Constraint) -> () {
        self.constraints.push(c)
    }

    pub fn satisfied(&self, pass: &Pass) -> bool {
        self.constraints.iter().all(|x| x.satisfied(pass))
    }
}

impl Pass {
    pub fn value_of(&self, s: &'static str) -> VarValue {
        *self.vars.get(s).unwrap_or(&0)
    }
}
