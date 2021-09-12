console.clear();

export class Person {
  name: string;
  constructor(name) {
    this.name = name;
  }
  toString() {
    return `My name is ${this.name}`;
  }
}

Person.prototype[Symbol.operatorSet] = Operators.create({
  "*"(a, b) {
    return `${a.name} got multiplied with ${b.name}`;
  }
}, {
  left: Number,
  "*"(num, person) {
    return `${person.name} got left-multiplied with ${num}`;
  }
}, {
  right: Number,
  "*"(person, num) {
    //console.log(Object.keys(this)); // globalThis
    return `${person.name} got right-multiplied with ${num}`;
  }
});

johndoe = new Person('John Doe');
php = new Person('php');
console.log(php * johndoe);
console.log(10 * php);
console.log(johndoe * 11); // right-multiplied
console.log(johndoe *= 10); // right-multiplied

- johndoe
