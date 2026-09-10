$ErrorActionPreference = "Continue"
$root = "d:/Program/HardwareClass/2.1_stm32/Projects/MobilePhone_F4"
Set-Location $root

$gcc = "D:\Applications\STM32_APPs\STM32CubeCLT\STM32CubeCLT_1.18.0\GNU-tools-for-STM32\bin\arm-none-eabi-gcc.exe"
$objs = Join-Path $root "build\chkobj"
if (!(Test-Path $objs)) { New-Item -ItemType Directory -Force -Path $objs | Out-Null }

$roots = @("App","Core","Drivers","Middlewares","My_Drivers")
$opts = @("-mcpu=cortex-m4","-mthumb","-mfpu=fpv4-sp-d16","-mfloat-abi=hard",
          "-DSTM32F411xE","-DUSE_HAL_DRIVER","-std=gnu11",
          "-Os","-ffunction-sections","-fdata-sections")
foreach ($r in $roots) {
    $opts += ("-I" + (Resolve-Path $r).Path)
    $opts += (Get-ChildItem -Recurse -Directory -Path $r |
              Where-Object { $_.FullName -notmatch 'PICTURE|examples|CMSIS_RTOS_V2' } |
              ForEach-Object { "-I" + $_.FullName })
}

$srcs = @()
foreach ($r in $roots) {
    $srcs += (Get-ChildItem -Recurse -File -Filter "*.c" -Path $r |
              Where-Object { $_.FullName -notmatch 'PICTURE|\\examples\\|CMSIS_RTOS_V2' } |
              ForEach-Object { $_.FullName })
}

$fail = 0
foreach ($s in $srcs) {
    $rel = $s.Substring($root.Length + 1) -replace '[\\/]', '_'
    $o = Join-Path $objs ($rel + ".o")
    if ((Test-Path $o) -and ((Get-Item $o).LastWriteTime -ge (Get-Item $s).LastWriteTime)) { continue }
    $a = $opts + @("-c", $s, "-o", $o)
    $out = & $gcc $a 2>&1 | Where-Object { $_ -match "error" }
    if ($out) {
        $fail++
        Write-Output ("FAIL " + $s)
        $out | Select-Object -First 4 | ForEach-Object { Write-Output ("    " + $_) }
    }
}
Write-Output ("compile fail count: " + $fail)

$startup = Join-Path $root "startup_stm32f411xe.s"
if (Test-Path $startup) {
    & $gcc @("-mcpu=cortex-m4","-mthumb","-mfpu=fpv4-sp-d16","-mfloat-abi=hard","-c",$startup,"-o",(Join-Path $objs "startup.o")) 2>&1 | Select-Object -First 5
}

$objList = Get-ChildItem -File -Filter "*.o" -Path $objs | ForEach-Object { $_.FullName }
Set-Content -Path "build\objlist.txt" -Value ($objList -replace '\\','/') -Encoding ASCII

$ldflags = @("-mcpu=cortex-m4","-mthumb","-mfpu=fpv4-sp-d16","-mfloat-abi=hard",
             "-specs=nano.specs","-T","STM32F411XX_FLASH.ld",
             "-Wl,--gc-sections","-Wl,-Map=build/chk.map")
$allargs = $ldflags + @("-Wl,@build/objlist.txt","-o","build/chk.elf","-lm")
& $gcc $allargs 2>&1 | Where-Object { $_ -notmatch "RWX" } | Select-Object -First 25

if (Test-Path "build/chk.elf") {
    & "D:\Applications\STM32_APPs\STM32CubeCLT\STM32CubeCLT_1.18.0\GNU-tools-for-STM32\bin\arm-none-eabi-size.exe" "build/chk.elf"
}
