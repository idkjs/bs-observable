type t('a);
[@bs.new] [@bs.module]
external make : ((SubscriptionObserver.t('a), unit) => unit) => t('a) =
  "zen-observable";

[@bs.send] external subscribe : (t('a), 'a => unit) => Subscription.t = "";

[@bs.send]
external forEach : (t('a), ('a, unit => unit) => unit) => Js.Promise.t(unit) =
  "";
