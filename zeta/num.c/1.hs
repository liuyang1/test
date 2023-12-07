import Criterion.Measurement

main = secs <$> time_ (print [0..10]) >>= print
