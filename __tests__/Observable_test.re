open Jest;
// let myMap = Js.Dict.empty();
// Js.Dict.set(myMap, "Allison", 10);

type person = {
  name: string,
  age: int,
  /* let's pretend our JS expects a null here */
};

let john = {name: "John", age: 20};
external toJson: 'a => Js.Json.t = "%identity";
let testObj = john;
let johnJson = {name: "John", age: 20};

describe("Observable", () => {
  testAsync("from", finish =>
    Observable.from([|0, 1, 2, 3|])
    ->(Observable.reduce((acc, value) => [value, ...acc], []))
    ->(
        Observable.subscribe(x =>
          Expect.expect(x) |> Expect.toEqual([3, 2, 1, 0]) |> finish
        )
      )
    ->ignore
  );
  testAsync("subscribe", finish =>
    Observable.make((observer: SubscriptionObserver.t(int)) => {
      observer->(SubscriptionObserver.next(10));
      ignore;
    })
    ->(
        Observable.subscribe(x =>
          Expect.expect(x) |> Expect.toBe(10) |> finish
        )
      )
    ->ignore
  );
  testAsync("subscribe string", finish =>
    Observable.make((observer: SubscriptionObserver.t(string)) => {
      observer->(SubscriptionObserver.next("message"));
      ignore;
    })
    ->(
        Observable.subscribe(x =>
          Expect.expect(x) |> Expect.toBe("message") |> finish
        )
      )
    ->ignore
  );

  testAsync("subscribe person", finish =>
    Observable.make((observer: SubscriptionObserver.t(person)) => {
      observer->(SubscriptionObserver.next(testObj));
      ignore;
    })
    ->(
        Observable.subscribe(x =>
          Expect.expect(x) |> Expect.toBe(testObj) |> finish
        )
      )
    ->ignore
  );
  testAsync("forEach", finish =>
    (
      Observable.from([|10|])
      ->(
          Observable.forEach((x, _) =>
            Expect.expect(x) |> Expect.toBe(10) |> ignore
          )
        )
      |> Js.Promise.then_(() => {
           finish(pass);
           Js.Promise.resolve();
         })
    )
    ->ignore
  );
  testAsync("map", finish =>
    Observable.from([|10|])
    ->(Observable.map(string_of_int))
    ->(
        Observable.subscribe(x =>
          Expect.expect(x) |> Expect.toBe("10") |> finish
        )
      )
    ->ignore
  );
  testAsync("filter", finish =>
    Observable.make((observer: SubscriptionObserver.t(int)) => {
      observer->(SubscriptionObserver.next(10));
      observer->(SubscriptionObserver.next(20));
      observer->(SubscriptionObserver.next(30));
      ignore;
    })
    ->(Observable.filter((==)(20)))
    ->(
        Observable.subscribe(x =>
          Expect.expect(x) |> Expect.toBe(20) |> finish
        )
      )
    ->ignore
  );
  testAsync("reduce", finish =>
    Observable.from([|10, 20, 30|])
    ->(Observable.reduce((acc, value) => acc ++ string_of_int(value), "00"))
    ->(
        Observable.subscribe(x =>
          Expect.expect(x) |> Expect.toBe("00102030") |> finish
        )
      )
    ->ignore
  );
  testAsync("concat", finish =>
    Observable.from([|10|])
    ->(
        Observable.concat([|
          Observable.from([|20|]),
          Observable.from([|30|]),
        |])
      )
    ->(Observable.reduce((acc, value) => [value, ...acc], []))
    ->(
        Observable.subscribe(x =>
          Expect.expect(x) |> Expect.toEqual([30, 20, 10]) |> finish
        )
      )
    ->ignore
  );
  testAsync("keepMap", finish =>
    Observable.from([|0, 1, 2, 3|])
    ->(Observable.keepMap(a => a mod 2 == 1 ? Some(a) : None))
    ->(Observable.reduce((acc, value) => [value, ...acc], []))
    ->(
        Observable.subscribe(x =>
          Expect.expect(x) |> Expect.toEqual([3, 1]) |> finish
        )
      )
    ->ignore
  );
});