/** This file is part of natlab.
 * https://github.com/mvasilkov/natlab
 * @license MIT+Ethics | Copyright (c) 2022, 2023 Mark Vasilkov
 * See https://github.com/mvasilkov/natlab/blob/master/LICENSE
 */
'use strict'

import { strict as assert } from 'node:assert'
import { spawn } from 'node:child_process'
import { dirname, join } from 'node:path'
import { platform } from 'node:process'
import { fileURLToPath } from 'node:url'
import { PerlinNoise } from 'natlib/noise/PerlinNoise.js'
import p from './permutation.json' assert { type: 'json' }

const python = platform === 'win32' ? 'python' : 'python3'
const modulePath = fileURLToPath(import.meta.url)
const __dirname = dirname(modulePath)

function pythonPerlin(x, y, z) {
    const p = spawn(python, [join(__dirname, 'rosettacode.py'), x, y, z])
    return new Promise((resolve, reject) => {
        p.stdout.on('data', data => {
            resolve(parseFloat(data.toString('utf8')))
        })
        p.stderr.on('data', data => {
            reject(data.toString('utf8'))
        })
    })
}

const perlin = new PerlinNoise(p.concat(p))

async function run() {
    for (let n = 0; n < 100; ++n) {
        const x = Math.random() * 200 - 100
        const y = Math.random() * 200 - 100
        const z = Math.random() * 200 - 100
        console.log(x, y, z)

        const a = perlin.noise3(x, y, z)
        const b = await pythonPerlin(x, y, z)
        assert(Math.abs(a - b) < Number.EPSILON, `natlib: ${a}, Python: ${b}`)
    }
}

run()
