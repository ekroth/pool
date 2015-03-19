#![allow(dead_code)]
#![allow(unstable)]
#![allow(unused_imports)]

mod constraint;
mod store;
mod solver;
mod var;

#[cfg(test)]
mod test {

    use std::ops::Range;
    use std::collections::HashMap;

    use constraint::Constraint;
    use store::Store;
    use store::Pass;
    use solver::Solver;
    use solver;
    use var::Var;

    fn init_store() -> (Vec<Var>, Store) {
        let mut store = Store::new();
        let nums = Range { start: 0, end: 10 };

        let s = Var { name: "S", range: nums };
        let e = Var { name: "E", range: nums };
        let n = Var { name: "N", range: nums };
        let d = Var { name: "D", range: nums };
        let m = Var { name: "M", range: nums };
        let o = Var { name: "O", range: nums };
        let r = Var { name: "R", range: nums };
        let y = Var { name: "Y", range: nums };

        let digits = vec![ s, e, n, d, m, o, r, y ];

        /* stupid copy... */
        store.impose(Constraint::AllDiffer(vec![ s, e, n, d, m, o, r, y ]));

        store.impose(Constraint::Linear(vec![
            (s, 1000),
            (e, 100),
            (n, 10),
            (d, 1),
            (m, 1000),
            (o, 100),
            (r, 10),
            (e, 1),
            (m, -10000),
            (o, -1000),
            (n, -100),
            (e, -10),
            (y, -1),
            ], 0));

        store.impose(Constraint::VarDiff(s, 0));
        store.impose(Constraint::VarDiff(m, 0));

        (digits, store)
    }

    #[test]
    fn can_satisfy_correct() {
        let solution = {
            let mut solved = HashMap::new();
            solved.insert("S", 9);
            solved.insert("E", 5);
            solved.insert("N", 6);
            solved.insert("D", 7);
            solved.insert("M", 1);
            solved.insert("O", 0);
            solved.insert("R", 8);
            solved.insert("Y", 2);
            Pass::new(solved)
        };

        let (_, store) = init_store();

        assert!(store.satisfied(&solution));
    }

    #[test]
    fn can_satisfy_invalid() {
        let solution = {
            let mut solved = HashMap::new();
            solved.insert("S", 2);
            solved.insert("E", 1);
            solved.insert("N", 6);
            solved.insert("D", 4);
            solved.insert("M", 1);
            solved.insert("O", 0);
            solved.insert("R", 8);
            solved.insert("Y", 2);
            Pass::new(solved)
        };

        let (_, store) = init_store();

        assert!(!store.satisfied(&solution));
    }

    #[test]
    fn worthless() {
        let s = solver::Worthless;
        let (_, store) = init_store();
        let res = s.solve(&store);
        assert!(res.is_none());
    }

    #[test]
    fn recursive() {
        let (digits, store) = init_store();
    }
}
