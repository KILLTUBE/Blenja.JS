export class Mat3 {
    m0: number;
    m1: number;
    m2: number;
    m3: number;
    m4: number;
    m5: number;
    m6: number;
    m7: number;
    m8: number;

    constructor() {
        this.m0 = 1;
        this.m1 = 0;
        this.m2 = 0;

        this.m3 = 0;
        this.m4 = 1;
        this.m5 = 0;

        this.m6 = 0;
        this.m7 = 0;
        this.m8 = 1;
    }

    clone(): Mat3 {
        var tmp = new Mat3();
        tmp.copy(this);
        return tmp;
    }

    copy(rhs: Mat3): Mat3 {
        this.m0 = rhs.m0;
        this.m1 = rhs.m1;
        this.m2 = rhs.m2;
        this.m3 = rhs.m3;
        this.m4 = rhs.m4;
        this.m5 = rhs.m5;
        this.m6 = rhs.m6;
        this.m7 = rhs.m7;
        this.m8 = rhs.m8;
        return this;
    }

    equals(rhs: Mat3): boolean {
        return (
            (this.m0 === rhs.m0) &&
            (this.m1 === rhs.m1) &&
            (this.m2 === rhs.m2) &&
            (this.m3 === rhs.m3) &&
            (this.m4 === rhs.m4) &&
            (this.m5 === rhs.m5) &&
            (this.m6 === rhs.m6) &&
            (this.m7 === rhs.m7) &&
            (this.m8 === rhs.m8)
        );
    }

    isIdentity(): boolean {
        return (
            (this.m0 === 1) &&
            (this.m1 === 0) &&
            (this.m2 === 0) &&
            (this.m3 === 0) &&
            (this.m4 === 1) &&
            (this.m5 === 0) &&
            (this.m6 === 0) &&
            (this.m7 === 0) &&
            (this.m8 === 1)
        );
    }

    setIdentity(): Mat3 {
        this.m0 = 1;
        this.m1 = 0;
        this.m2 = 0;

        this.m3 = 0;
        this.m4 = 1;
        this.m5 = 0;
        
        this.m6 = 0;
        this.m7 = 0;
        this.m8 = 1;

        return this;
    }

    transpose(): Mat3 {
        var tmp: number;

        tmp = this.m1;
        this.m1 = this.m3;
        this.m3 = tmp;

        tmp = this.m2;
        this.m2 = this.m6;
        this.m6 = tmp;

        tmp = this.m5;
        this.m5 = this.m7;
        this.m7 = tmp;

        return this;
    }
}
