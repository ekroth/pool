use store::{ Store, Pass };
use var::Var;

pub trait Solver {
    fn solve(&self, store: &Store) -> Option<Pass>;
}

pub struct Worthless;

impl Solver for Worthless {
    #[allow(unused_variables)]
    fn solve(&self, store: &Store) -> Option<Pass> {
        None
    }
}

pub struct Recursive<'a> {
    vars: &'a Vec<Var>,
}

impl<'a> Recursive<'a> {
    fn new(vars: &'a Vec<Var>) -> Recursive<'a> {
        Recursive { vars: vars }
    }
}

impl<'a> Solver for Recursive<'a> {
    fn solve(&self, store: &Store) -> Option<Pass> {
        fn solve_it(store: &Store, pass: Pass, rest: &[Var]) -> Option<Pass> {
            if rest.len() == 0 {
                if store.satisfied(&pass) {
                    Some(pass)
                } else { None }
            } else {
                /* gen some other values from pass */
                let Pass { vars } = pass;
                None
            }
        }

        None
    }
}
