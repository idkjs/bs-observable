type t('a);
[@bs.send] external next : (t('a), 'a) => unit = "next";
[@bs.send] external error : (t('a), 'a) => unit = "error";
[@bs.send] external complete : (t('a), unit) => unit = "complete";
