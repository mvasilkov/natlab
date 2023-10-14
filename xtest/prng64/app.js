/** This file is part of natlab.
 * https://github.com/mvasilkov/natlab
 * @license MIT+Ethics | Copyright (c) 2022, 2023 Mark Vasilkov
 * See https://github.com/mvasilkov/natlab/blob/master/LICENSE
 */
'use strict'

import { createRequire } from 'node:module'
import { Thrust } from 'natlib/prng/Thrust.js'
import vectors from './vectors.js'

const require = createRequire(import.meta.url)
const nativeThrust = require('./build/Release/nativeThrust.node')

// Thrust (native)
for (const [start, vector] of Object.entries(vectors)) {
    nativeThrust.setState(BigInt(start))
    vector.forEach((a, n) => {
        a = BigInt(a)
        const b = nativeThrust.getUint64()

        if (a !== b) {
            console.log(`!!! Thrust (native) start=${start} n=${n} a=${a} b=${b}`)
            process.exit(1)
        }
    })
}

// Thrust (natlib)
for (const [start, vector] of Object.entries(vectors)) {
    const r = new Thrust(BigInt(start))
    vector.forEach((a, n) => {
        a = BigInt(a)
        const b = r.randomUint64()

        if (a !== b) {
            console.log(`!!! Thrust (natlib) start=${start} n=${n} a=${a} b=${b}`)
            process.exit(1)
        }
    })
}
