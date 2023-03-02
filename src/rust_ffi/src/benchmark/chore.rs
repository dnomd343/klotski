use std::fmt;

#[derive(Debug)]
pub(crate) struct Duration {
    picos: u64
}

impl Duration {
    #[inline]
    fn to_ps(&self) -> f64 {
        self.picos as f64 / (1 as f64)
    }

    #[inline]
    fn to_ns(&self) -> f64 {
        self.picos as f64 / (1_000 as f64)
    }

    #[inline]
    fn to_us(&self) -> f64 {
        self.picos as f64 / (1_000_000 as f64)
    }

    #[inline]
    fn to_ms(&self) -> f64 {
        self.picos as f64 / (1_000_000_000 as f64)
    }
}

impl Duration {
    #[inline]
    fn to_ps_str(&self) -> String {
        format!("{:.03}ps", self.to_ps())
    }

    #[inline]
    fn to_ns_str(&self) -> String {
        format!("{:.03}ns", self.to_ns())
    }

    #[inline]
    fn to_us_str(&self) -> String {
        format!("{:.03}us", self.to_us())
    }

    #[inline]
    fn to_ms_str(&self) -> String {
        format!("{:.03}ms", self.to_ms())
    }
}

impl Duration {
    #[inline]
    #[allow(dead_code)]
    pub(crate) fn from_ps(ps: f64) -> Duration {
        Duration {
            picos: (ps * (1 as f64)) as u64
        }
    }

    #[inline]
    #[allow(dead_code)]
    pub(crate) fn from_ns(ns: f64) -> Duration {
        Duration {
            picos: (ns * (1_000 as f64)) as u64
        }
    }

    #[inline]
    #[allow(dead_code)]
    pub(crate) fn from_us(us: f64) -> Duration {
        Duration {
            picos: (us * (1_000_000 as f64)) as u64
        }
    }

    #[inline]
    #[allow(dead_code)]
    pub(crate) fn from_ms(ms: f64) -> Duration {
        Duration {
            picos: (ms * (1_000_000_000 as f64)) as u64
        }
    }
}

impl fmt::Display for Duration {
    fn fmt(&self, f: &mut fmt::Formatter<'_>) -> fmt::Result {
        write!(f, "{}", if self.picos < 1000 {
            self.to_ps_str()
        } else if self.picos < 1_000_000 {
            self.to_ns_str()
        } else if self.picos < 1_000_000_000 {
            self.to_us_str()
        } else {
            self.to_ms_str()
        })
    }
}
