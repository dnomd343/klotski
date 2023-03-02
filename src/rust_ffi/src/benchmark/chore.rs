#[derive(Debug)]
pub(crate) struct Duration {
    picos: u64
}

impl Duration {
    #[allow(dead_code)]
    pub(crate) fn from_ns(ns: f64) -> Duration {
        Duration {
            picos: (ns * (1_000 as f64)) as u64
        }
    }

    #[allow(dead_code)]
    pub(crate) fn from_us(us: f64) -> Duration {
        Duration {
            picos: (us * (1_000_000 as f64)) as u64
        }
    }

    #[allow(dead_code)]
    pub(crate) fn from_ms(ms: f64) -> Duration {
        Duration {
            picos: (ms * (1_000_000_000 as f64)) as u64
        }
    }

    #[allow(dead_code)]
    pub(crate) fn from_s(s: f64) -> Duration {
        Duration {
            picos: (s * (1_000_000_000_000 as i64 as f64)) as u64
        }
    }

    #[allow(dead_code)]
    pub(crate) fn to_ns(&self) -> String {
        let tmp = self.picos as f64 / (1_000 as f64);
        format!("{:.03}ns", tmp)
    }

    #[allow(dead_code)]
    pub(crate) fn to_us(&self) -> String {
        let tmp = self.picos as f64 / (1_000_000 as f64);
        format!("{:.03}us", tmp)
    }

    #[allow(dead_code)]
    pub(crate) fn to_ms(&self) -> String {
        let tmp = self.picos as f64 / (1_000_000_000 as f64);
        format!("{:.03}ms", tmp)
    }

    #[allow(dead_code)]
    pub(crate) fn to_s(&self) -> String {
        let tmp = self.picos as f64 / (1_000_000_000_000 as i64 as f64);
        format!("{:.03}s", tmp)
    }
}
