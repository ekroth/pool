use store::Store;
use store::Pass;
use var::Var;
use var::VarValue;

#[derive(Show)]
pub enum Constraint {
    AllDiffer(Vec<Var>),
    VarDiff(Var, VarValue),
    Linear(Vec<(Var, VarValue)>, VarValue), /* equals */
}

impl Constraint {
    pub fn satisfied(&self, pass: &Pass) -> bool {
        match *self {
            Constraint::AllDiffer(ref vars) => {
                vars.iter().all(|x| {
                    let dup = vars.iter().any(|y| {
                        y.name != x.name && (pass.value_of(y.name) == pass.value_of(x.name))
                    });

                    !dup
                })
            },

            Constraint::VarDiff(ref var, not_expected) => {
                pass.value_of(var.name) != not_expected
            },

            Constraint::Linear(ref vars, expected) => {
                let sum = vars.iter()
                    .map(|&(var, val)| pass.value_of(var.name) * val)
                    .fold(0, |a, x| a + x);

                sum == expected
            },
        }
    }
}
