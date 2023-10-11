/** This file is part of natlab.
 * https://github.com/mvasilkov/natlab
 * @license MIT+Ethics | Copyright (c) 2022, 2023 Mark Vasilkov
 * See https://github.com/mvasilkov/natlab/blob/master/LICENSE
 */
'use strict'

import { createRequire } from 'module'

import { Mulberry32 } from 'natlib/prng/Mulberry32.js'
import { SplitMix32 } from 'natlib/prng/SplitMix32.js'

const require = createRequire(import.meta.url)
const nativeMulberry32 = require('./build/Release/nativeMulberry32.node')
const nativeSplitMix32 = require('./build/Release/nativeSplitMix32.node')

const initialState = 0

const r = new Mulberry32(initialState)
const s = new SplitMix32(initialState)

nativeMulberry32.setState(initialState)
nativeSplitMix32.setState(initialState)

// Mulberry32
for (let n = 0; n < 2 ** 32; ++n) {
    const a = r.randomUint32()
    const b = nativeMulberry32.getUint32()

    if (a !== b) {
        console.log(`!!! Mulberry32 n=${n} a=${a} b=${b}`)
        break
    }

    if (n % 10_000_000 === 0) console.log(n)
}

// SplitMix32
for (let n = 0; n < 2 ** 32; ++n) {
    const a = s.randomUint32()
    const b = nativeSplitMix32.getUint32()

    if (a !== b) {
        console.log(`!!! SplitMix32 n=${n} a=${a} b=${b}`)
        break
    }

    if (n % 10_000_000 === 0) console.log(n)
}
